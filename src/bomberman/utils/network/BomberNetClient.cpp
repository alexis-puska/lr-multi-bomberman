#include "BomberNetClient.h"

BomberNetClient BomberNetClient::m_instance = BomberNetClient();

TCPsocket BomberNetClient::tcpsock = NULL;
SDLNet_SocketSet BomberNetClient::socketset = NULL;
bool BomberNetClient::alive = false;
int BomberNetClient::errorCode = 0;
int BomberNetClient::errorValue = 0;
int BomberNetClient::requestNumber = 0;
ClientViewer * BomberNetClient::viewer = NULL;

int BomberNetClient::nbPlayerOnThisClient = 0;
unsigned short * BomberNetClient::keystate = NULL;

bool BomberNetClient::keystateThreadAlive = false;
bool BomberNetClient::keychange[16];
unsigned short BomberNetClient::previousPlayerKeystate[16];



BomberNetClient& BomberNetClient::Instance() {
	return m_instance;
}

BomberNetClient::BomberNetClient() {
	alive = false;
	net_thread = NULL;

	for (int i = 0; i < 16; i++) {
		keychange[i] = false;
		previousPlayerKeystate[i] = false;
	}
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
	if (viewer != NULL) {
		delete viewer;
		viewer = NULL;
	}
	stopKeystateThread();
	SDLNet_Quit();
}

int BomberNetClient::net_thread_main(void *data) {
	BomberNetClient *bomberNet = ((BomberNetClient *) data);
	while (bomberNet->isAlive()) {
		BomberNetClient::errorCode = bomberNet->handleNet();
		if (BomberNetClient::errorCode > 0) {
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
int BomberNetClient::connectClient(SDL_Surface * vout_buf) {
	errorCode = 0;
	IPaddress serverIP;

	fprintf(stderr, "%s\n", GameConfig::Instance().getIpString());
	viewer = new ClientViewer(vout_buf);
	startKeystateThread();
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
	char data[7];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x00;
	data[5] = GameConfig::Instance().getNbPlayerOfClient();
	data[6] = '\0';
	if (SDLNet_CheckSockets(BomberNetClient::socketset, 0) >= 0) {
		SDLNet_TCP_Send(BomberNetClient::tcpsock, &data, 7);
		requestNumber++;
	}
}

void BomberNetClient::sendDisconnection() {
	char data[7];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x01;
	data[5] = GameConfig::Instance().getNbPlayerOfClient();
	data[6] = '\0';
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
	data[5] = GameConfig::Instance().getNbPlayerOfClient();
	int pos = 6;
	for (int i = 0; i < GameConfig::Instance().getNbPlayerOfClient(); i++) {
		SDLNet_Write16(keystate[i], data + pos);
		pos += 2;
	}
	data[pos] = '\0';
	if (SDLNet_CheckSockets(BomberNetClient::socketset, 0) >= 0) {
		SDLNet_TCP_Send(BomberNetClient::tcpsock, &data, pos);
		requestNumber++;
	}
}

int BomberNetClient::handleNet() {
	char data[1024];
	int len;

	int active = SDLNet_CheckSockets(BomberNetClient::socketset, 0);

	if (active > 0) {
		if (SDLNet_SocketReady(BomberNetClient::tcpsock)) {
			memset(data, 0, sizeof data);
			len = SDLNet_TCP_Recv(tcpsock, (char *) data, 1024);
			if (len <= 0) {
				SDLNet_TCP_Close(tcpsock);
				BomberNetClient::tcpsock = NULL;
				SDLNet_TCP_DelSocket(BomberNetClient::socketset, BomberNetClient::tcpsock);
			} else {
				//fprintf(stderr, "Receive from Server : %s\n", data);
				//int requestNumber = SDLNet_Read32(data);
				int type = data[4];

				//fprintf(stderr, "request number : %i, %x, %x\n", requestNumber, type, data[5], data[6]);
				switch (type) {
					case 0:
						switch (data[5]) {
							case 0:
								//fprintf(stderr, "Nombre de place disponible : %i\n", data[6]);
								if (data[6] > GameConfig::Instance().getNbPlayerOfClient()) {
									sendNbPlayerClient();
								} else {
									//fprintf(stderr, "pas assez de place ! %i places libres\n", data[6]);
									errorValue = data[6];
									return 6;
								}
								break;
							case 1:
								//fprintf(stderr, "serveur plein !");
								return 1;
							case 2:
								//fprintf(stderr, "serveur en jeu!");
								return 2;
						}
						break;
					case 1:
						switch (data[5]) {
							case 1:
								//fprintf(stderr, "Nombre de place disponible : %i", data[6]);
								if (data[6] == GameConfig::Instance().getNbPlayerOfClient()) {
									//fprintf(stderr, "Server accept all player");
									return 0;
								} else {
									//fprintf(stderr, "le serveur ne renvoi pas le nombre correct de joueur");
									return 3;
								}
								break;
							case 2:
								//fprintf(stderr, "le serveur n'acceptera que  %i joueurs", data[6]);
								errorValue = data[6];
								return 4;
								break;
						}
						break;
					case 2:
						fprintf(stderr, "Nombre d'element dans la requette : %i", data[5]);
						BomberNetClient::viewer->decode(data);
						return 0;
						break;
				}

			}
		}
	} else if (active == 0) {
		//no activity on socket
	} else {
		return 5;
	}
	return 0;
}

int BomberNetClient::keystateThread(void *data) {
	fprintf(stderr, "Starting thread keystate...\n");
	nbPlayerOnThisClient = GameConfig::Instance().getNbPlayerOfClient();
	keystate = GameConfig::Instance().getKeystate(0);
	while (BomberNetClient::keystateThreadAlive) {
		SDL_Delay(1);
		checkKeystate();
	}
	fprintf(stderr,"je meurs\n");
	return 0;
}

void BomberNetClient::startKeystateThread() {
	BomberNetClient::keystateThreadAlive = true;
	keystate_thread = SDL_CreateThread(keystateThread, "keystate thread", this);
	fprintf(stderr, "Starting looking keystate...\n");
}

void BomberNetClient::stopKeystateThread() {
	fprintf(stderr, "Stoping looking keystate...%i \n", nbPlayerOnThisClient);
	if (BomberNetClient::keystateThreadAlive) {
		BomberNetClient::keystateThreadAlive = false;
		int treadResult = 0;
		SDL_WaitThread(keystate_thread, &treadResult);
		fprintf(stderr, "Looking keystate stopped ! : %i\n", treadResult);
	}
}

bool BomberNetClient::checkKeystate() {
	bool found = false;
	for (int i = 0; i < nbPlayerOnThisClient; i++) {
		if (previousPlayerKeystate[i] != keystate[i]) {
			found = true;
			keychange[i] = true;
			previousPlayerKeystate[i] = keystate[i];
		} else {
			keychange[i] = false;
		}
	}
	if(found){
		if(BomberNetClient::alive){
			BomberNetClient::sendKeystate();
		}
	}
}
