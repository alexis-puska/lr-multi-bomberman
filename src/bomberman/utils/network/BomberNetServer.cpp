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
	initBuffer();
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
	fprintf(stderr, "send screen change command %i\n", screen);
	char data[2];
	data[0] = 0x01; //screen type
	data[1] = screen;
	concatBuffer(data, 2);
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
	char data[25];
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
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 25);
		requestNumber++;
	}
}

void BomberNetServer::sendSpriteType() {
	char data[25];
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
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 25);
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
	char data[29];
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
		SDLNet_TCP_Send(bomber[it->first].sock, &data, 29);
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
	if ((unsigned)(bufferPosition + length) > sizeof buffer) {
		sendBuffer();
		initBuffer();
	}
	memcpy(buffer + bufferPosition, src, length);
	bufferElement++;
	bufferPosition += length;
	buffer[bufferPosition] = '\0';
}

/***********************
 *close buffer and send
 **********************/
void BomberNetServer::sendBuffer() {
	buffer[5] = bufferElement;
	buffer[bufferPosition] = '\0';
	std::map<int, int>::iterator it;
	for (it = connexionHuman.begin(); it != connexionHuman.end(); ++it) {
		SDLNet_TCP_Send(bomber[it->first].sock, &buffer, bufferPosition);
		requestNumber++;
	}

	if(bufferElement < 5){
		fprintf(stderr, "send buffer %i %i\n", bufferElement, bufferPosition);
	}
	initBuffer();
}
/***********************
 *   init buffer
 **********************/
void BomberNetServer::initBuffer() {
	memset(buffer, 0, sizeof buffer);
	SDLNet_Write32(requestNumber, buffer);
	buffer[4] = 0x02;
	buffer[6] = '\0';
	bufferPosition = 6;
	bufferElement = 0;
}

/*********************
 * game information
 *********************/

void BomberNetServer::sendGameInfo(int time, bool newCycle, int gameState) {
	char tmp[5];
	tmp[0] = 3;
	SDLNet_Write16(time, tmp + 1);
	tmp[3] = newCycle ? 1 : 0;
	tmp[4] = gameState;
	concatBuffer(tmp, 5);
}

void BomberNetServer::sendTab(int * tab) {
	fprintf(stderr, "send Tab update request\n");
	char tmp[736];
	tmp[0] = 4;
	for (int i = 0; i < 735; i++) {
		tmp[i + 1] = *(tab + i);
	}
	concatBuffer(tmp, 736);
}

void BomberNetServer::sendTabBonus(int * tabBonus) {
	char tmp[736];
	tmp[0] = 5;
	for (int i = 0; i < 735; i++) {
		tmp[i + 1] = *(tabBonus + i);
	}
	concatBuffer(tmp, 736);
}

void BomberNetServer::sendPlayer(float posX, float posY, int sprite, int louis, int spaceship, bool inverse) {
	char tmp[11];
	tmp[0] = 6;
	SDLNet_Write16((int) (posX * 100.0), tmp + 1);
	SDLNet_Write16((int) (posY * 100.0), tmp + 3);
	SDLNet_Write16(sprite, tmp + 5);
	SDLNet_Write16(louis, tmp + 7);
	SDLNet_Write16(spaceship, tmp + 9);
	tmp[10] = inverse ? 1 : 0;
	concatBuffer(tmp, 11);
}

void BomberNetServer::sendPlayerState() {
	char tmp[17];
	tmp[0] = 7;
	for (int i = 0; i < 16; i++) {
		tmp[i + 1] = GameConfig::Instance().isPlayerAlive(i) ? 1 : 0;
	}
	concatBuffer(tmp, 17);
}

void BomberNetServer::sendNewEmptyElement(int idx) {
	char tmp[3];
	tmp[0] = 8;
	SDLNet_Write16(idx, tmp + 1);
	concatBuffer(tmp, 3);
}

void BomberNetServer::sendRail(int idx, int sprite) {
	char tmp[4];
	tmp[0] = 9;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = sprite;
	concatBuffer(tmp, 4);
}

void BomberNetServer::sendTrolley(float posX, float posY, int sprite) {
	char tmp[6];
	tmp[0] = 10;
	SDLNet_Write16((int) (posX * 100.0), tmp + 1);
	SDLNet_Write16((int) (posY * 100.0), tmp + 3);
	tmp[5] = sprite;
	concatBuffer(tmp, 6);
}

void BomberNetServer::sendButton(int idx, int sprite) {
	char tmp[4];
	tmp[0] = 11;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = sprite;
	concatBuffer(tmp, 4);
}

void BomberNetServer::sendBurnLouis(float posX, float posY) {
	char tmp[5];
	tmp[0] = 12;
	SDLNet_Write16((int) (posX * 100.0), tmp + 1);
	SDLNet_Write16((int) (posY * 100.0), tmp + 3);
	concatBuffer(tmp, 5);
}

void BomberNetServer::sendBurnBonus(int idx) {
	char tmp[3];
	tmp[0] = 13;
	SDLNet_Write16(idx, tmp + 1);
	concatBuffer(tmp, 3);
}

void BomberNetServer::sendburnWall(int idx) {
	char tmp[3];
	tmp[0] = 14;
	SDLNet_Write16(idx, tmp + 1);
	concatBuffer(tmp, 3);
}

void BomberNetServer::sendExplosion(int idx, int type) {
	char tmp[4];
	tmp[0] = 15;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = type;
	concatBuffer(tmp, 4);
}

void BomberNetServer::sendPopBonus(int idx) {
	char tmp[3];
	tmp[0] = 16;
	SDLNet_Write16(idx, tmp + 1);
	concatBuffer(tmp, 3);
}

void BomberNetServer::sendSuddenDeath(int idx) {
	char tmp[3];
	tmp[0] = 17;
	SDLNet_Write16(idx, tmp + 1);
	concatBuffer(tmp, 3);
}

void BomberNetServer::sendBombe(float posX, float posY, int type, int sprite) {
	char tmp[7];
	tmp[0] = 18;
	SDLNet_Write16((int) (posX * 100.0), tmp + 1);
	SDLNet_Write16((int) (posY * 100.0), tmp + 3);
	tmp[5] = type;
	tmp[6] = sprite;
	concatBuffer(tmp, 7);
}

void BomberNetServer::sendHole(int idx, int sprite) {
	char tmp[4];
	tmp[0] = 19;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = sprite;
	concatBuffer(tmp, 4);
}

void BomberNetServer::sendMine(int idx, int sprite) {
	char tmp[4];
	tmp[0] = 20;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = sprite;
	concatBuffer(tmp, 4);
}

void BomberNetServer::sendTeleporter(int idx, int sprite) {
	char tmp[4];
	tmp[0] = 21;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = sprite;
	concatBuffer(tmp, 4);
}

void BomberNetServer::sendGhost(float posX, float posY) {
	char tmp[5];
	tmp[0] = 22;
	SDLNet_Write16((int) (posX * 100.0), tmp + 1);
	SDLNet_Write16((int) (posY * 100.0), tmp + 3);
	concatBuffer(tmp, 5);
}

/**********************
 * audio
 *********************/

void BomberNetServer::sendMusique(int idx, bool lect) {
	char tmp[3];
	tmp[0] = 23;
	tmp[1] = idx;
	tmp[2] = lect ? 1 : 0;
	concatBuffer(tmp, 3);
}

void BomberNetServer::sendSound(int sound, int channel, bool lect) {
	char tmp[4];
	tmp[0] = 24;
	tmp[1] = sound;
	tmp[2] = channel;
	tmp[3] = lect ? 1 : 0;
	concatBuffer(tmp, 4);
}

/**********************
 * bonus
 *********************/

void BomberNetServer::sendBonusAppear(int idx, int type) {
	char tmp[4];
	tmp[0] = 25;
	SDLNet_Write16(idx, tmp + 1);
	tmp[3] = type;
	concatBuffer(tmp, 4);

}
void BomberNetServer::sendBonusDisapear(int idx) {
	char tmp[3];
	tmp[0] = 26;
	SDLNet_Write16(idx, tmp + 1);
	concatBuffer(tmp, 3);
}
