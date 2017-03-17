#include "BomberNetServer.h"

BomberNetServer BomberNetServer::m_instance = BomberNetServer();

TCPsocket BomberNetServer::servsock = NULL;
SDLNet_SocketSet BomberNetServer::socketset = NULL;
bool BomberNetServer::alive = false;
int BomberNetServer::nbClientConnected = 0;
std::map<int, int> BomberNetServer::connexionHuman;
int BomberNetServer::requestNumber = 0;

static struct {
		int active;
		int startIndexNetKeystate;
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
	memset(buffer, 0, sizeof buffer);
	bufferPosition = 6;
	bufferElement = 0;
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

bool BomberNetServer::createServerSocket() {
	IPaddress serverIP;
	/* Create the server socket */
	SDLNet_ResolveHost(&serverIP, NULL, GameConfig::Instance().getPortValue());
	fprintf(stderr, "Server IP: %x, %d\n", serverIP.host, serverIP.port);
	servsock = SDLNet_TCP_Open(&serverIP);
	nbClientConnected = 0;
	if (servsock == NULL) {
		fprintf(stderr, "Couldn't create server socket: %s\n", SDLNet_GetError());
		cleanup();
		return false;
	}
	GameConfig::Instance().resetNumberNetPlayer();
	SDLNet_TCP_AddSocket(socketset, servsock);
	return true;
}

bool BomberNetServer::createTcpServer() {
	initSDLNet();
	allocateSockets();
	return createServerSocket();
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

int BomberNetServer::net_thread_main(void *data) {
	fprintf(stderr, "Starting thread server...\n");
	BomberNetServer *bomberNet = ((BomberNetServer *) data);
	while (bomberNet->isAlive()) {
		int activite = SDLNet_CheckSockets(socketset, 1);
		if (activite >= 0) {
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
		sendServerFull(newsock);
		SDLNet_TCP_Close(newsock);
	} else if (!GameConfig::Instance().getAcceptClient()) {
		sendServerInGame(newsock);
		SDLNet_TCP_Close(newsock);
	} else {
		addInactiveSocket(which, newsock);
		nbClientConnected++;
		bomber[which].active = 1;
		sendSlotAvailable(which);
	}
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

int BomberNetServer::getNbClientConnected() {
	return nbClientConnected;
}

void BomberNetServer::linkKeystate() {
	int sum = 0;
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {

		bomber[it->first].startIndexNetKeystate = sum;
		sum += it->second;
	}
}

void BomberNetServer::HandleClient(int which) {
	char data[1024];
	memset(data, 0, sizeof data);
	/* Has the connection been closed? */
	if (SDLNet_TCP_Recv(bomber[which].sock, data, 1024) <= 0) {

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
			nbClientConnected--;
			GameConfig::Instance().removeNetPlayer(connexionHuman[which]);
			connexionHuman.erase(which);
		}
		deleteConnection(which);
	} else {
		decode(data, which);
	}
	memset(data, 0, sizeof data);
}

void BomberNetServer::sendSlotAvailable(int which) {
	char data[8];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 16 - GameConfig::Instance().getNbReservedPlayerServer() - GameConfig::Instance().getNbNetPlayer();
	data[7] = 0x00;
	SDLNet_TCP_Send(bomber[which].sock, &data, 8);
	requestNumber++;
}

void BomberNetServer::sendServerFull(TCPsocket newsock) {
	char data[7];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x00;
	data[5] = 0x01;
	data[6] = '\0';
	SDLNet_TCP_Send(newsock, &data, 7);
	requestNumber++;
}

void BomberNetServer::sendServerInGame(TCPsocket newsock) {
	char data[7];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x00;
	data[5] = 0x02;
	data[6] = '\0';
	SDLNet_TCP_Send(newsock, &data, 7);
	requestNumber++;
}

void BomberNetServer::sendAcknoledgementOfClientPlayer(int which) {
	char data[8];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x01;
	data[5] = 0x01;
	data[6] = connexionHuman[which];
	data[7] = '\0';
	SDLNet_TCP_Send(bomber[which].sock, &data, 8);
	requestNumber++;
}

void BomberNetServer::sendErrorSlotAvailable(int which) {
	char data[8];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x00;
	data[5] = 0x02;
	data[6] = 16 - GameConfig::Instance().getNbReservedPlayerServer() - GameConfig::Instance().getNbNetPlayer();
	data[7] = '\0';
	SDLNet_TCP_Send(bomber[which].sock, &data, 8);
	requestNumber++;
}

void BomberNetServer::decode(char data[1024], int which) {
	//int requestNumber = SDLNet_Read32(data);
	int type = data[4];
	std::map<int, int>::iterator it;
	int sum = 0;
	int pos;
	switch (type) {
		case 0:
			//Valid the client connexion
			for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
				sum += it->second;
			}
			if (data[5] <= 16 - sum - GameConfig::Instance().getNbReservedPlayerServer()) {
				GameConfig::Instance().addNetPlayer(data[5]);
				connexionHuman[which] = data[5];
				sendAcknoledgementOfClientPlayer(which);
				sendNbConnected();
			} else {
				sendErrorSlotAvailable(which);
				deleteConnection(which);
			}
			break;
		case 1:
			//deconnection du client demandé
			if (data[5] == connexionHuman[which]) {
				deleteConnection(which);
			}
			break;
		case 2:
			//reception evenement manette
			for (int j = 0; j < data[5]; j++) {
				pos = 6 + (2 * j);
				GameConfig::Instance().setKeyPressedForNetPlayer(bomber[which].startIndexNetKeystate + j, SDLNet_Read16(data + pos));
			}
			break;
	}
}

/**********************
 *
 * GAME CONFIG REQUEST
 *
 *********************/

void BomberNetServer::sendChangeScreenCommand(int screen) {
	fprintf(stderr, "send screen change command %i", screen);
	char data[9];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	data[5] = 0x01; //number of object
	data[6] = 0x01; //screen type
	data[7] = screen;
	data[8] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 9);
		requestNumber++;
	}
}

void BomberNetServer::sendNbConnected() {
	char data[11];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	data[5] = 0x01; //number of object
	data[6] = 0x02; //screen type
	data[7] = 0x01;
	data[8] = BomberNetServer::Instance().getNbClientConnected();
	data[9] = GameConfig::Instance().getNbNetPlayer() + GameConfig::Instance().getNbReservedPlayerServer();
	data[10] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 11);
		requestNumber++;
	}
}

void BomberNetServer::sendPlayerType() {
	char data[24];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	data[5] = 0x01; //number of object
	data[6] = 0x02; //screen type
	data[7] = 0x02;
	int pos = 8;
	for (int i = 0; i < nbPlayer; i++) {
		data[pos] = GameConfig::Instance().getPlayerType(i);
		pos++;
	}
	data[pos] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 24);
		requestNumber++;
	}
}

void BomberNetServer::sendSpriteType() {
	char data[24];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	data[5] = 0x01; //number of object
	data[6] = 0x02; //screen type
	data[7] = 0x03;
	int pos = 8;
	for (int i = 0; i < nbPlayer; i++) {
		data[pos] = GameConfig::Instance().getPlayerSpriteType(i);
		pos++;
	}
	data[pos] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 24);
		requestNumber++;
	}
}

void BomberNetServer::sendGameOption() {
	char data[13];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	data[5] = 0x01; //number of object
	data[6] = 0x02; //screen type
	data[7] = 0x04;

	data[8] = GameConfig::Instance().isSuddentDeathMode() ? 1 : 0;
	data[9] = GameConfig::Instance().isBadBomberMode() ? 1 : 0;
	data[10] = GameConfig::Instance().getIALevel();
	data[11] = GameConfig::Instance().getTimeOfGame();
	data[12] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 13);
		requestNumber++;
	}
}

void BomberNetServer::sendLevelInfo() {
	char data[28];
	memset(data, 0, sizeof data);
	SDLNet_Write32(requestNumber, data);
	data[4] = 0x02;
	data[5] = 0x01; //number of object
	data[6] = 0x02; //screen type
	data[7] = 0x05;

	data[8] = GameConfig::Instance().getLevel();
	data[9] = GameConfig::Instance().getVariante();
	data[10] = GameConfig::Instance().getBombe();
	data[11] = GameConfig::Instance().getStrenghtBombe();
	data[12] = GameConfig::Instance().isCustomBonus() ? 1 : 0;

	int pos = 13;
	for (int i = 0; i < nbTypeBonus; i++) {
		data[pos] = GameConfig::Instance().getBonus(i);
		pos++;
	}
	data[pos] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 28);
		requestNumber++;
	}
}


/**********************
 *
 *     GAME REQUEST
 *
 *********************/

/********************
 *   concat buffer
 ********************/
void BomberNetServer::concatBuffer(char * src, int length) {
	strncpy(buffer + bufferPosition, src, length);
	bufferElement++;
	bufferPosition += length;
}

/***********************
 *close buffer and send
 **********************/
void BomberNetServer::sendBuffer(){
	buffer[5] = bufferElement;
	buffer[bufferPosition] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &buffer, 28);
		requestNumber++;
	}
}
/***********************
 *   init buffer
 **********************/
void BomberNetServer::initBuffer() {
	memset(buffer, 0, sizeof buffer);
	SDLNet_Write32(requestNumber, buffer);
	buffer[4] = 0x02;
	//buffer[5] = ???, change value when send the buffer
	bufferPosition = 6;
	bufferElement = 0;
}




//tab
//tabBonus

//time/new cycle/playerState/gameState(pause/in game/end)
//player position
//new empty element


//rail
//trolley
//button
//burnlouis
//burnbonus
//burnwall
//exposion
//popbonus
//suddentdeath
//bombe
//hole
//mine
//player
//teleporter