#include "BomberNetClient.h"

BomberNetClient BomberNetClient::m_instance = BomberNetClient();

TCPsocket BomberNetClient::tcpsock = NULL;
SDLNet_SocketSet BomberNetClient::socketset = NULL;
bool BomberNetClient::alive = false;
int BomberNetClient::requestNumber = 0;

BomberNetClient& BomberNetClient::Instance() {
	return m_instance;
}

BomberNetClient::BomberNetClient() {
	alive = false;
	net_thread = NULL;
}

BomberNetClient::~BomberNetClient() {
	alive = false;
	cleanup();
}

void BomberNetClient::initSDLNet() {

	if (SDLNet_Init() < 0) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		fprintf(stderr, "ERROR\n");
	} else {
		fprintf(stderr, "SUCCES\n");
	}
}

void BomberNetClient::allocateSocket() {
	/* Allocate the socket set */
	socketset = SDLNet_AllocSocketSet(1);
	if (socketset == NULL) {
		fprintf(stderr, "Couldn't create socket set: %s\n", SDLNet_GetError());
		cleanup();
	}
}

void BomberNetClient::createTcpClient() {
	initSDLNet();
	allocateSocket();
}

void BomberNetClient::cleanup() {
	if (tcpsock != NULL) {
		SDLNet_TCP_Close(tcpsock);
		tcpsock = NULL;
	}
	if (socketset != NULL) {
		SDLNet_FreeSocketSet(socketset);
		socketset = NULL;
	}
	SDLNet_Quit();
}

int BomberNetClient::net_thread_main(void *data) {
	BomberNetClient *bomberNet = ((BomberNetClient *) data);
	while (bomberNet->isAlive()) {

		if(!bomberNet->handleNet()){
			break;
		}
	}
	return 0;
}

bool BomberNetClient::isAlive() {
	return alive;
}

/**************************************
 * CLIENT
 *************************************/
int BomberNetClient::connectClient() {
	IPaddress serverIP;

	fprintf(stderr, "%s\n", GameConfig::Instance().getIpString());
	SDLNet_ResolveHost(&serverIP, GameConfig::Instance().getIpString(), GameConfig::Instance().getPortValue());

	if (serverIP.host == INADDR_NONE) {
		fprintf(stderr, "Couldn't resolve hostname\n");
		cleanup();
	} else {
		/* If we fail, it's okay, the GUI shows the problem */
		tcpsock = SDLNet_TCP_Open(&serverIP);
		if (tcpsock == NULL) {
			fprintf(stderr, "Connect failed\n");
			cleanup();
			return 1;
		} else {
			fprintf(stderr, "Connection established !\n");
			socketset = SDLNet_AllocSocketSet(1);
			SDLNet_TCP_AddSocket(socketset, tcpsock);
			alive = true;
			net_thread = SDL_CreateThread(net_thread_main, "tcp client thread", tcpsock);
			return 0;
		}
	}
	return 2;
}

void BomberNetClient::disconnectClient() {
	if (alive) {
		alive = false;
		int treadResult = 0;
		SDL_WaitThread(net_thread, &treadResult);
		fprintf(stderr, "Client disconnected %i\n", treadResult);
		cleanup();
	}
}

void BomberNetClient::sendNbPlayerClient() {
	char data[8];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x00;
	SDLNet_Write16(GameConfig::Instance().getNbPlayerOfClient(), data + 5);
	data[7]='\0';
	if (SDLNet_CheckSockets(BomberNetClient::socketset, 0) >= 0) {
		SDLNet_TCP_Send(BomberNetClient::tcpsock, &data, 8);
		requestNumber++;
	}
}

void BomberNetClient::sendDisconnection() {
	char data[8];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x01;
	SDLNet_Write16(GameConfig::Instance().getNbPlayerOfClient(), data + 5);
	data[7]='\0';
	if (SDLNet_CheckSockets(BomberNetClient::socketset, 0) >= 0) {
		SDLNet_TCP_Send(BomberNetClient::tcpsock, &data, 7);
		requestNumber++;
	}
}

void BomberNetClient::sendKeystate() {
	char data[38];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	SDLNet_Write16(GameConfig::Instance().getNbPlayerOfClient(), data + 5);
	int pos = 9;
	for (int i = 0; i < GameConfig::Instance().getNbPlayerOfClient(); i++) {
		SDLNet_Write16(GameConfig::Instance().getKeystate(i), data + pos);
		pos += 2;
	}
	data[pos]='\0';
	if (SDLNet_CheckSockets(BomberNetClient::socketset, 0) >= 0) {
		SDLNet_TCP_Send(BomberNetClient::tcpsock, &data, pos);
		requestNumber++;
	}
}

bool BomberNetClient::handleNet() {
	char data[512];
	int len, pos;

	int active = SDLNet_CheckSockets(BomberNetClient::socketset, 0);

	if (active > 0) {
		if (SDLNet_SocketReady(BomberNetClient::tcpsock)) {
			memset(data, 0, sizeof data);
			len = SDLNet_TCP_Recv(tcpsock, (char *) data, 512);
			if (len <= 0) {
				SDLNet_TCP_Close(tcpsock);
				BomberNetClient::tcpsock = NULL;
				SDLNet_TCP_DelSocket(BomberNetClient::socketset, BomberNetClient::tcpsock);
			} else {
				fprintf(stderr, "Receive from Server : %s\n", data);

						int requestNumber = SDLNet_Read32(data);
						int type = data[5];
						fprintf(stderr, "request number : %i, %x, %x, %i", requestNumber, type, data[6], data[7]);
						sendNbPlayerClient();
			}
		}
	}else if(active == 0){
		//no activity on socket
	}else{
		return false;
	}
	return true;
}

