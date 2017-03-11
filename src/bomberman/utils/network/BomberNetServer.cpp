#include "BomberNetServer.h"

BomberNetServer BomberNetServer::m_instance = BomberNetServer();

TCPsocket BomberNetServer::servsock = NULL;
SDLNet_SocketSet BomberNetServer::socketset = NULL;
bool BomberNetServer::alive = false;

static struct {
		int active;
		TCPsocket sock;
		IPaddress peer;
		Uint8 name[256 + 1];
} bomber[GAME_MAXPEOPLE];

BomberNetServer& BomberNetServer::Instance() {
	return m_instance;
}

BomberNetServer::BomberNetServer() {
	alive = false;
	net_thread = NULL;
}

BomberNetServer::~BomberNetServer() {
	alive = false;
	cleanup();
	fprintf(stderr, "Close network system\n");
}

void BomberNetServer::initSDLNet() {
	fprintf(stderr, "Init network system : ");
	if (SDLNet_Init() < 0) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		fprintf(stderr, "ERROR\n");
	} else {
		fprintf(stderr, "SUCCES\n");
	}
}

void BomberNetServer::cleanup() {
	if (servsock != NULL) {
		SDLNet_TCP_Close(servsock);
		servsock = NULL;
	}
	if (socketset != NULL) {
		SDLNet_FreeSocketSet(socketset);
		socketset = NULL;
	}
	SDLNet_Quit();
}

void BomberNetServer::allocateSockets() {
	/* Allocate the socket set */
	socketset = SDLNet_AllocSocketSet(15);
	if (socketset == NULL) {
		fprintf(stderr, "Couldn't create socket set: %s\n", SDLNet_GetError());
		cleanup();
	}
}

void BomberNetServer::createServerSocket() {
	IPaddress serverIP;
	/* Create the server socket */
	SDLNet_ResolveHost(&serverIP, NULL, GameConfig::Instance().getPortValue());
	fprintf(stderr, "Server IP: %x, %d\n", serverIP.host, serverIP.port);
	servsock = SDLNet_TCP_Open(&serverIP);
	if (servsock == NULL) {
		fprintf(stderr, "Couldn't create server socket: %s\n", SDLNet_GetError());
		cleanup();
	}
	SDLNet_TCP_AddSocket(socketset, servsock);
}

void BomberNetServer::createTcpServer() {
	initSDLNet();
	allocateSockets();
	createServerSocket();
}

void BomberNetServer::startServer() {
	alive = true;
	net_thread = SDL_CreateThread(net_thread_main, "tcp server thread", servsock);
	fprintf(stderr, "Starting server...\n");
}

void BomberNetServer::addInactiveSocket(int which, TCPsocket newsock) {
	/* Add socket as an inactive person */
	bomber[which].sock = newsock;
	bomber[which].peer = *SDLNet_TCP_GetPeerAddress(newsock);
	SDLNet_TCP_AddSocket(socketset, bomber[which].sock);
}

void BomberNetServer::roomFull(TCPsocket newsock) {
	char tmp[47] = "This server of LR-Multi-Bomberman is full !!!\n";
	SDLNet_TCP_Send(newsock, tmp, 47);
	SDLNet_TCP_Close(newsock);
}

int BomberNetServer::net_thread_main(void *data) {
	fprintf(stderr, "Starting thread server...\n");
	BomberNetServer *bomberNet = ((BomberNetServer *) data);
	while (bomberNet->isAlive()) {
		SDLNet_CheckSockets(socketset, ~0);
		/* Check for new connections */
		if (SDLNet_SocketReady(servsock)) {
			bomberNet->HandleServer();
		}
		/* Check for events on existing clients */
		for (int i = 0; i < GameConfig::Instance().getNbClientServer(); ++i) {
			if (SDLNet_SocketReady(bomber[i].sock)) {
				bomberNet->HandleClient(i);
			}
		}
	}
	return 0;
}

void BomberNetServer::stopServer() {
	fprintf(stderr, "Start killing server\n");
	if (alive) {
		alive = false;
		for (int i = 0; i < GameConfig::Instance().getNbClientServer(); i++) {
			fprintf(stderr, "...");
			deleteConnection(i);
		}
		cleanup();
		int treadResult = 0;
		SDL_WaitThread(net_thread, &treadResult);
		fprintf(stderr, "\nServer KILLED : %i\n", treadResult);

	}
}

void BomberNetServer::HandleServer(void) {
	int which;
	TCPsocket newsock;

	newsock = SDLNet_TCP_Accept(servsock);

	IPaddress *remote_ip;
	Uint32 ip;

	remote_ip = SDLNet_TCP_GetPeerAddress(newsock);
	ip = SDL_SwapBE32(remote_ip->host);
	if (!remote_ip) {
		printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
		printf("This may be a server socket.\n");
	} else {

		fprintf(stderr, "New Player %i.%i.%i.%i %i\n", ip >> 24, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff, remote_ip->port);
	}

	if (newsock == NULL) {
		return;
	}

	/* Look for unconnected person slot */
	for (which = 0; which < GameConfig::Instance().getNbClientServer(); ++which) {
		if (!bomber[which].sock) {
			break;
		}
	}
	if (which == GameConfig::Instance().getNbClientServer()) {
		findInactivePersonSlot(which);
	}
	if (which == GameConfig::Instance().getNbClientServer()) {
		roomFull(newsock);
	} else {
		addInactiveSocket(which, newsock);
		char tmp[42] = "Welcome to LR-Multi-Bomberman Server !!!\n";
		bomber[which].active = 1;
		SDLNet_TCP_Send(newsock, tmp, 42);
	}
}

void BomberNetServer::HandleClient(int which) {
	char data[512];
	memset(data, 0, sizeof data);
	/* Has the connection been closed? */
	if (SDLNet_TCP_Recv(bomber[which].sock, data, 512) <= 0) {

		IPaddress *remote_ip;
		Uint32 ip;
		remote_ip = SDLNet_TCP_GetPeerAddress(bomber[which].sock);
		bomber[which].active = 0;
		ip = SDL_SwapBE32(remote_ip->host);
		if (!remote_ip) {
			printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
			printf("This may be a server socket.\n");
		} else {
			bomber[which].active = 0;
			fprintf(stderr, "Player left : %i.%i.%i.%i %i\n", ip >> 24, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff, remote_ip->port);
		}
		deleteConnection(which);
	} else {
		switch (data[0]) {
			case 0x04:
				bomber[which].active = 0;
				break;
			default:
				fprintf(stderr, "Receive from client %i : %s", which, data);
				bomber[which].active = 1;
				break;
		}

	}
	memset(data, 0, sizeof data);
}

void BomberNetServer::deleteConnection(int which) {
	SDLNet_TCP_DelSocket(BomberNetServer::socketset, bomber[which].sock);
	SDLNet_TCP_Close(bomber[which].sock);
	bomber[which].sock = NULL;
}

void BomberNetServer::findInactivePersonSlot(int &which) {
	for (which = 0; which < GameConfig::Instance().getNbClientServer(); ++which) {
		if (bomber[which].sock && !bomber[which].active) {
			/* Kick them out.. */
			unsigned char data = GAME_BYE;
			SDLNet_TCP_Send(bomber[which].sock, &data, 1);
			SDLNet_TCP_DelSocket(BomberNetServer::socketset, bomber[which].sock);
			SDLNet_TCP_Close(bomber[which].sock);
			break;
		}
	}
}

bool BomberNetServer::isAlive() {
	return alive;
}

void BomberNetServer::sendLine() {
	unsigned char data[11] = "ABCDEFGHI\n";
	for (int i = 0; i < GameConfig::Instance().getNbClientServer(); i++) {
		if (bomber[i].active) {
			SDLNet_TCP_Send(bomber[i].sock, &data, 11);
		}
	}
}
