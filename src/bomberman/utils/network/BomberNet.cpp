#include "BomberNet.h"

BomberNet BomberNet::m_instance = BomberNet();

TCPsocket BomberNet::servsock = NULL;
SDLNet_SocketSet BomberNet::socketset = NULL;
bool BomberNet::alive = false;

static struct {
		int active;
		TCPsocket sock;
		IPaddress peer;
		Uint8 name[256 + 1];
} bomber[GAME_MAXPEOPLE];

BomberNet& BomberNet::Instance() {
	return m_instance;
}

BomberNet::BomberNet() {
	alive = false;
	net_thread = NULL;
}

BomberNet::~BomberNet() {
	alive = false;
	cleanup();
	fprintf(stderr, "Close network system\n");
}

void BomberNet::initSDLNet() {
	fprintf(stderr, "Init network system : ");
	if (SDLNet_Init() < 0) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		fprintf(stderr, "ERROR\n");
	} else {
		fprintf(stderr, "SUCCES\n");
	}
}

void BomberNet::cleanup() {
	if (servsock != NULL) {
		SDLNet_TCP_Close(servsock);
		servsock = NULL;
	}
	if (socketset != NULL) {
		SDLNet_FreeSocketSet(socketset);
		socketset = NULL;
	}
	SDLNet_Quit();
	SDL_Quit();
}

void BomberNet::allocateSockets() {
	/* Allocate the socket set */
	socketset = SDLNet_AllocSocketSet(15);
	if (socketset == NULL) {
		fprintf(stderr, "Couldn't create socket set: %s\n", SDLNet_GetError());
		cleanup();
	}
}

void BomberNet::createServerSocket() {
	IPaddress serverIP;

	/* Create the server socket */
	SDLNet_ResolveHost(&serverIP, NULL, GAME_PORT);
	printf("Server IP: %x, %d\n", serverIP.host, serverIP.port);
	servsock = SDLNet_TCP_Open(&serverIP);
	if (servsock == NULL) {
		fprintf(stderr, "Couldn't create server socket: %s\n", SDLNet_GetError());
		cleanup();
	}
	SDLNet_TCP_AddSocket(socketset, servsock);
}

void BomberNet::createTcpServer() {
	initSDLNet();
	allocateSockets();
	createServerSocket();
}

void BomberNet::startServer() {
	alive = true;
	net_thread = SDL_CreateThread(net_thread_main, "test", servsock);

	fprintf(stderr, "server starting...\n");
}

void BomberNet::addInactiveSocket(int which, TCPsocket newsock) {
	/* Add socket as an inactive person */
	bomber[which].sock = newsock;
	bomber[which].peer = *SDLNet_TCP_GetPeerAddress(newsock);
	SDLNet_TCP_AddSocket(socketset, bomber[which].sock);
}

void BomberNet::roomFull(TCPsocket newsock) {
	/* No more room... */
	char data = GAME_BYE;
	SDLNet_TCP_Send(newsock, &data, 1);
	SDLNet_TCP_Close(newsock);
}

int BomberNet::net_thread_main(void *data) {
	BomberNet *bomberNet = ((BomberNet *) data);
	fprintf(stderr, "server starting... phase (2)\n");
	while (bomberNet->isAlive()) {
		SDLNet_CheckSockets(socketset, ~0);

		/* Check for new connections */
		if (SDLNet_SocketReady(servsock)) {
			bomberNet->HandleServer();
		}

		/* Check for events on existing clients */
		for (int i = 0; i < GAME_MAXPEOPLE; ++i) {
			if (SDLNet_SocketReady(bomber[i].sock)) {
				bomberNet->HandleClient(i);
			}
		}
	}
	return 0;
}

void BomberNet::stopServer() {
	fprintf(stderr, "Start killing server\n");
	if (alive) {
		alive = false;
		for(int i=0;i<GAME_MAXPEOPLE;i++){
			fprintf(stderr, "...");
			deleteConnection(i);
		}
		cleanup();
		int treadResult = 0;
		SDL_WaitThread(net_thread, &treadResult);
		fprintf(stderr, "\nServer KILLED : %i\n", treadResult);

	}
}

void BomberNet::HandleServer(void) {
	int which;
	TCPsocket newsock;

	newsock = SDLNet_TCP_Accept(servsock);




	IPaddress *remote_ip;
	Uint32 ip;

	remote_ip=SDLNet_TCP_GetPeerAddress(newsock);
	ip=SDL_SwapBE32(remote_ip->host);
	if(!remote_ip) {
	    printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
	    printf("This may be a server socket.\n");
	}
	else {
		fprintf(stderr, "New Player %i.%i.%i.%i %i\n", ip>>24, (ip>>16)&0xff, (ip>>8)&0xff, ip&0xff, remote_ip->port);
	}


	if (newsock == NULL) {
		return;
	}

	/* Look for unconnected person slot */
	for (which = 0; which < GAME_MAXPEOPLE; ++which) {
		if (!bomber[which].sock) {
			break;
		}
	}
	if (which == GAME_MAXPEOPLE) {
		findInactivePersonSlot(which);
	}
	if (which == GAME_MAXPEOPLE) {
		roomFull(newsock);
	} else {
		addInactiveSocket(which, newsock);
		char tmp[11] = "Bienvenue\0";
		SDLNet_TCP_Send(newsock, tmp, 10);
	}
}

void BomberNet::HandleClient(int which) {
	char data[512];
	/* Has the connection been closed? */
	if (SDLNet_TCP_Recv(bomber[which].sock, data, 512) <= 0) {

		IPaddress *remote_ip;
			Uint32 ip;
		remote_ip=SDLNet_TCP_GetPeerAddress(bomber[which].sock);
			ip=SDL_SwapBE32(remote_ip->host);
			if(!remote_ip) {
			    printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
			    printf("This may be a server socket.\n");
			}
			else {
				fprintf(stderr, "Player left : %i.%i.%i.%i %i\n", ip>>24, (ip>>16)&0xff, (ip>>8)&0xff, ip&0xff, remote_ip->port);
			}

		notifyAllConnectionClosed(data, which);
		deleteConnection(which);

	} else {
		switch (data[0]) {
			case GAME_HELLO: {

				memcpy(&bomber[which].peer.port, &data[GAME_HELLO_PORT], 2);
				memcpy(bomber[which].name, &data[GAME_HELLO_NAME], 256);
				bomber[which].name[256] = 0;

				/* Notify all active clients */
				for (int i = 0; i < GAME_MAXPEOPLE; ++i) {
					if (bomber[i].active) {
						SendNew(which, i);
					}
				}

				/* Notify about all active clients */
				bomber[which].active = 1;
				for (int i = 0; i < GAME_MAXPEOPLE; ++i) {
					if (bomber[i].active) {
						SendNew(i, which);
					}
				}

				/*Tell player which one in the slot they are */
				SendID(which);
			}
				break;
			default: {
				/* Yay!  An active connection */
				memcpy(&bomber[which].peer.port, &data[GAME_HELLO_PORT], 2);
				memcpy(bomber[which].name, &data[GAME_HELLO_NAME], 256);
				bomber[which].name[256] = 0;

				/* Notify all active clients */
				for (int i = 0; i < GAME_MAXPEOPLE; ++i) {
					if (bomber[i].active) {
						SendNew(which, i);
					}
				}

				/* Notify about all active clients */
				bomber[which].active = 1;
				for (int i = 0; i < GAME_MAXPEOPLE; ++i) {
					if (bomber[i].active) {
						SendNew(i, which);
					}
				}

				/*Tell player which one in the slot they are */
				SendID(which);
			}
				break;
		}

	}
}

void deleteConnection(int which) {
	SDLNet_TCP_DelSocket(BomberNet::socketset, bomber[which].sock);
	SDLNet_TCP_Close(bomber[which].sock);
	bomber[which].sock = NULL;
}

/* Send a "new client" notification */
void BomberNet::SendNew(int about, int to) {
	char data[512];
	int n;
	n = strlen((char *) bomber[about].name) + 1;
	data[0] = GAME_ADD;
	data[GAME_ADD_SLOT] = about;
	memcpy(&data[GAME_ADD_HOST], &bomber[about].peer.host, 4);
	memcpy(&data[GAME_ADD_PORT], &bomber[about].peer.port, 2);
	data[GAME_ADD_NLEN] = n;
	memcpy(&data[GAME_ADD_NAME], bomber[about].name, n); //if more info, add it here on next line ie appearance/colour
	SDLNet_TCP_Send(bomber[to].sock, data, GAME_ADD_NAME + n);
}

void BomberNet::SendID(int which) {
	char data[512];
	data[0] = GAME_ID;
	data[1] = which;
	SDLNet_TCP_Send(bomber[which].sock, data, GAME_ID_LEN + 1);
}

void BomberNet::notifyAllConnectionClosed(char data[], int which) {
	if (bomber[which].active) {
		bomber[which].active = 0;
		data[0] = GAME_DEL;
		data[GAME_DEL_SLOT] = which;
		for (int i = 0; i < GAME_MAXPEOPLE; ++i) {
			if (bomber[i].active) {
				SDLNet_TCP_Send(bomber[i].sock, data, GAME_DEL_LEN);
			}
		}
	}
}

void BomberNet::deleteConnection(int which) {
	SDLNet_TCP_DelSocket(socketset, bomber[which].sock);
	SDLNet_TCP_Close(bomber[which].sock);
	bomber[which].sock = NULL;
}

void BomberNet::findInactivePersonSlot(int &which) {
	for (which = 0; which < GAME_MAXPEOPLE; ++which) {
		if (bomber[which].sock && !bomber[which].active) {
			/* Kick them out.. */
			unsigned char data = GAME_BYE;
			SDLNet_TCP_Send(bomber[which].sock, &data, 1);
			SDLNet_TCP_DelSocket(BomberNet::socketset, bomber[which].sock);
			SDLNet_TCP_Close(bomber[which].sock);
			break;
		}
	}
}

bool BomberNet::isAlive() {
	return alive;
}

void BomberNet::sendLine(){
	unsigned char data = 0x41;
	for(int i=0;i<GAME_MAXPEOPLE;i++){
		if (bomber[i].active) {
			SDLNet_TCP_Send(bomber[i].sock, &data, 1);
		}
	}
}