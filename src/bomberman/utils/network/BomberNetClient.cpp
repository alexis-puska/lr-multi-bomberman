#include "BomberNetClient.h"

BomberNetClient BomberNetClient::m_instance = BomberNetClient();

TCPsocket BomberNetClient::tcpsock = NULL;
SDLNet_SocketSet BomberNetClient::socketset = NULL;
bool BomberNetClient::alive = false;

static struct {
		int active;
		TCPsocket sock;
		IPaddress peer;
		Uint8 name[256 + 1];
} bomber[GAME_MAXPEOPLE];

BomberNetClient& BomberNetClient::Instance() {
	return m_instance;
}

BomberNetClient::BomberNetClient() {
	alive = false;
	net_thread = NULL;
	socketset = SDLNet_AllocSocketSet(1);
	if (socketset == NULL) {
		//fprintf(stderr, "Couldn't create socket set: %s\n", SDLNet_GetError());
		cleanup();
	}
}

BomberNetClient::~BomberNetClient() {
	alive = false;
	cleanup();
	fprintf(stderr, "Close network system\n");
}

void BomberNetClient::initSDLNet() {
	fprintf(stderr, "Init network system : ");
	if (SDLNet_Init() < 0) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		fprintf(stderr, "ERROR\n");
	} else {
		fprintf(stderr, "SUCCES\n");
	}
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
	SDL_Quit();
}

int BomberNetClient::net_thread_main(void *data) {
	fprintf(stderr, "Starting thread client...\n");
	BomberNetClient *bomberNet = ((BomberNetClient *) data);
	while (bomberNet->isAlive()) {

		bomberNet->handleNet();
	}
	return 0;
}

void BomberNetClient::handleNet() {
	char data[512];
	int pos, len;
	int used;
	const int timeout = 0;
	int active = SDLNet_CheckSockets(BomberNetClient::socketset, 0);
	// If there is activity.
	if (active > 0) {
		if (SDLNet_SocketReady(BomberNetClient::tcpsock)) {
			len = SDLNet_TCP_Recv(tcpsock, (char *) data, 512);
			if (len <= 0) {
				SDLNet_TCP_Close(tcpsock);
				BomberNetClient::tcpsock = NULL;
				SDLNet_TCP_DelSocket(BomberNetClient::socketset, BomberNetClient::tcpsock);
			} else {
				fprintf(stderr, "%s", data);
//		pos = 0;
//		while (len > 0) {
//			used = HandleServerData(&data[pos]);
//			pos += used;
//			len -= used;
//			if (used == 0) {
//				len = 0;
//			}
//		}
			}
		}
	}
}

bool BomberNetClient::isAlive() {
	return alive;
}

void BomberNetClient::sendLine() {
	unsigned char data[11] = "ABCDEFGHI\n";
	SDLNet_TCP_Send(BomberNetClient::tcpsock, &data, 11);
}

/**************************************
 * CLIENT
 *************************************/
void BomberNetClient::connectClient() {

	IPaddress serverIP;


	SDLNet_ResolveHost(&serverIP, "localhost", GAME_PORT);

	if (serverIP.host == INADDR_NONE) {
		fprintf(stderr, "Couldn't resolve hostname\n");
	} else {
		/* If we fail, it's okay, the GUI shows the problem */
		tcpsock = SDLNet_TCP_Open(&serverIP);
		if (tcpsock == NULL) {
			fprintf(stderr, "Connect failed\n");
		} else {
			fprintf(stderr, "Connection established !\n");
			alive = true;
			SDLNet_TCP_AddSocket(socketset, tcpsock);
			net_thread = SDL_CreateThread(net_thread_main, "tcp client thread", tcpsock);
		}
	}
}

void BomberNetClient::disconnectClient() {
	if (tcpsock != NULL) {
		SDLNet_TCP_Close(tcpsock);
		tcpsock = NULL;
	}
	if (socketset != NULL) {
		SDLNet_FreeSocketSet(socketset);
		socketset = NULL;
	}
}

