#include "GameConfig.h"

GameConfig GameConfig::m_instance = GameConfig();

GameConfig& GameConfig::Instance() {
	return m_instance;
}

GameConfig::GameConfig() {
	srand (time(NULL));level = 0;
	variante = 0;
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	suddenDeathMode = false;
	badBomberMode = false;
	IALevel = 1;
	gameMode = 0;
	numberOfNetPlayer=0;
	nbReservedPlayerServer =1;
	ipTab[0]=192;
	ipTab[1]=168;
	ipTab[2]=147;
	ipTab[3]=133;
	port = 7000;
	nbPlayerOfClient = 1;
	nbClientServer = nbClientMax;
	timeOfGame = timeOfGameDefault;

	nbplayerInGame = 0;
	for(int i = 0; i < nbTypeBonus; i++) {
		bonus[i] = 0;
	}
	for(int i = 0; i < nbPlayer; i++) {
		playerType[i] = CPU;
		playerColor[i] = 0;
		playerSpriteType[i] = 0;
	}
	playerType[0] = 0;
	resetPlayerScore();
	copyLevelBonus();
}

GameConfig::~GameConfig() {
}

void GameConfig::incLevel() {
	level++;
	if (level > LevelService::Instance().getNumberOfLevels() - 1) {
		level = 0;
	}
	if (variante > LevelService::Instance().getLevel(level)->getVariantesSize() - 1) {
		variante = LevelService::Instance().getLevel(level)->getVariantesSize() - 1;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}

void GameConfig::decLevel() {
	level--;
	if (level < 0) {
		level = LevelService::Instance().getNumberOfLevels() - 1;
	}
	if (variante > LevelService::Instance().getLevel(level)->getVariantesSize() - 1) {
		variante = LevelService::Instance().getLevel(level)->getVariantesSize() - 1;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}

int GameConfig::getLevel() {
	return level;
}

void GameConfig::incVariante() {
	variante++;
	if (variante > LevelService::Instance().getLevel(level)->getVariantesSize() - 1) {
		variante = 0;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}

void GameConfig::decVariante() {
	variante--;
	if (variante < 0) {
		variante = LevelService::Instance().getLevel(level)->getVariantesSize() - 1;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}

int GameConfig::getVariante() {
	return variante;
}

void GameConfig::incBombe() {
	nbBombe++;
	if (nbBombe > nbBombeMax) {
		nbBombe = nbBombeMin;
	}
}

void GameConfig::decBombe() {
	nbBombe--;
	if (nbBombe < nbBombeMin) {
		nbBombe = nbBombeMax;
	}
}

int GameConfig::getBombe() {
	return nbBombe;
}

void GameConfig::incStrengthBombe() {
	strengthBombe++;
	if (strengthBombe > bombeStrenghtMax) {
		strengthBombe = bombeStrenghtMin;
	}
}

void GameConfig::decStrengthBombe() {
	strengthBombe--;
	if (strengthBombe < bombeStrenghtMin) {
		strengthBombe = bombeStrenghtMax;
	}
}

int GameConfig::getStrenghtBombe() {
	return strengthBombe;
}

void GameConfig::incBonus(int idx) {
	customBonus = true;
	if ((sumTotalBonus() + 1) > nbMaxBonusTotal) {
		return;
	}
	bonus[idx]++;
	if (bonus[idx] > nbMaxBonusCat) {
		bonus[idx] = nbMaxBonusCat;
	}
}

void GameConfig::inc5Bonus(int idx) {
	customBonus = true;
	if ((sumTotalBonus() + 5) > nbMaxBonusTotal) {
		return;
	}
	bonus[idx] = bonus[idx] + 5;
	if (bonus[idx] > nbMaxBonusCat) {
		bonus[idx] = nbMaxBonusCat;
	}
}

void GameConfig::decBonus(int idx) {
	customBonus = true;
	bonus[idx]--;
	if (bonus[idx] < 0) {
		bonus[idx] = 0;
	}
}

void GameConfig::dec5Bonus(int idx) {
	customBonus = true;
	bonus[idx] = bonus[idx] - 5;
	if (bonus[idx] < 0) {
		bonus[idx] = 0;
	}
}

int GameConfig::getBonus(int idx) {
	return bonus[idx];
}

int GameConfig::sumTotalBonus() {
	int total = 0;
	for (int i = 0; i < nbTypeBonus; i++) {
		total += bonus[i];
	}
	return total;
}

void GameConfig::switchCustomBonus() {
	if (customBonus) {
		customBonus = false;
		copyLevelBonus();
	} else {
		customBonus = true;
	}
}

bool GameConfig::isCustomBonus() {
	return customBonus;
}

void GameConfig::switchSuddenDeathMode() {
	if (suddenDeathMode) {
		suddenDeathMode = false;
	} else {
		suddenDeathMode = true;
	}
}

bool GameConfig::isSuddentDeathMode() {
	return suddenDeathMode;
}

void GameConfig::switchBadBomberMode() {
	if (badBomberMode) {
		badBomberMode = false;
	} else {
		badBomberMode = true;
	}
}

bool GameConfig::isBadBomberMode() {
	return badBomberMode;
}

void GameConfig::incIALevel() {
	IALevel++;
	if (IALevel > nbMaxIALevel) {
		IALevel = 1;
	}
}

void GameConfig::decIALevel() {
	IALevel--;
	if (IALevel < 1) {
		IALevel = nbMaxIALevel;
	}
}

int GameConfig::getIALevel() {
	return IALevel;
}

void GameConfig::incTimeOfGame() {
	if (timeOfGame == timeOfGameInf) {
		timeOfGame = timeOfGameMin;
		return;
	}
	timeOfGame++;
	if (timeOfGame > timeOfGameMax) {
		timeOfGame = timeOfGameInf;
	}
}
void GameConfig::decTimeOfGame() {
	if (timeOfGame == timeOfGameInf) {
		timeOfGame = timeOfGameMax;
		return;
	}
	timeOfGame--;
	if (timeOfGame < timeOfGameMin) {
		timeOfGame = timeOfGameInf;
	}
}

int GameConfig::getTimeOfGame() {
	return timeOfGame;
}

void GameConfig::setNbPlayerInGame(int n) {
	nbplayerInGame = n;
}

int GameConfig::getNbPlayerInGame() {
	int nbplayerInGame = 0;
	for (int i = 0; i < 16; i++) {
		if (playerType[i] < 2) {
			nbplayerInGame++;
		}
	}
	return nbplayerInGame;
}

void GameConfig::incPlayerType(int idx) {
	if (idx != 0) {
		bool netSet = netPlayerAllSet();
		playerType[idx]++;
		if (playerType[idx] > 2) {
			if (playerType[idx] > 3) {
				playerType[idx] = 0;
			} else {
				if (netSet) {
					playerType[idx] = 0;
				} else {
					playerType[idx] = 3;
				}
			}
		}
	}
}

void GameConfig::decPlayerType(int idx) {
	if (idx != 0) {
		bool netSet = netPlayerAllSet();

		playerType[idx]--;
		if (playerType[idx] < 0) {
			if (netSet) {
				playerType[idx] = 2;
			} else {
				playerType[idx] = 3;
			}
		}
	}
}

void GameConfig::setPlayerType(int idx, int val) {
	playerType[idx] = val;
}

int GameConfig::getPlayerType(int idx) {
	return playerType[idx];
}

void GameConfig::setPlayerColor(int idx, int val) {
	playerColor[idx] = val;
}

int GameConfig::getPlayerColor(int idx) {
	return playerColor[idx];
}

void GameConfig::generatePlayerSpriteTypeforCPU() {
	for (int i = 0; i < 16; i++) {
		if (playerType[i] == 1) {
			playerSpriteType[i] = (rand() % nbTypePlayer);
		}
	}
}
void GameConfig::generateColorPlayer() {
	for (int i = 0; i < 16; i++) {
		playerColor[i] = (rand() % nbColorPlayer);
	}
}

void GameConfig::incPlayerSpriteType(int idx) {
	playerSpriteType[idx]++;
	if (playerSpriteType[idx] > nbTypePlayer - 1) {
		playerSpriteType[idx] = 0;
	}
}

void GameConfig::decPlayerSpriteType(int idx) {
	playerSpriteType[idx]--;
	if (playerSpriteType[idx] < 0) {
		playerSpriteType[idx] = nbTypePlayer - 1;
	}
}

void GameConfig::setPlayerSpriteType(int idx, int val) {
	playerSpriteType[idx] = val;
}

int GameConfig::getPlayerSpriteType(int idx) {
	return playerSpriteType[idx];
}

void GameConfig::incPlayerScore(int idx) {
	playerScore[idx]++;
}

int GameConfig::getPlayerScore(int idx) {
	return playerScore[idx];
}

void GameConfig::resetPlayerScore() {
	for (int i = 0; i < nbPlayer; i++) {
		playerScore[i] = 0;
	}
}

void GameConfig::setPlayerDead(int idx) {
	playerStatus[idx] = false;
}

void GameConfig::setPlayerAlive(int idx) {
	playerStatus[idx] = true;
}

int GameConfig::isPlayerAlive(int idx) {
	return playerStatus[idx];
}

void GameConfig::resetPlayerStatus() {
	for (int i = 0; i < nbPlayer; i++) {
		playerStatus[i] = 0;
	}
}

void GameConfig::copyLevelBonus() {
	for (int i = 0; i < nbTypeBonus; i++) {
		bonus[i] = LevelService::Instance().getLevel(level)->getVariantes(variante)->getBonus(i);
	}
}

void GameConfig::printPlayerConfiguration() {
	for (int i = 0; i < 16; i++) {
		fprintf(stderr, "%i %2it %2ic %2is %2isc %2ist\n", i, playerType[i], playerColor[i], playerSpriteType[i], playerScore[i], playerStatus[i]);
	}
}

void GameConfig::updatePlayerPosition(int player, float x, float y) {
	playerCord[player][0] = x;
	playerCord[player][1] = y;
}

void GameConfig::updatePlayerPosY(int player, float y) {
	playerCord[player][1] = y;
}

void GameConfig::updatePlayerPosX(int player, float x) {
	playerCord[player][0] = x;
}

float GameConfig::getPlayerPosX(int player) {
	return playerCord[player][0];
}

float GameConfig::getPlayerPosY(int player) {
	return playerCord[player][1];
}

int GameConfig::getPlayerIndex(int player) {
	return int(floor(playerCord[player][0]) + floor(playerCord[player][1]) * sizeX);
}

void GameConfig::resetPlayerCord() {
	for (int i = 0; i < nbPlayer; i++) {
		playerCord[i][0] = -1;
		playerCord[i][1] = -1;
	}
}

/*************************
 *
 * CONFIGURATION GAME MODE
 *
 *************************/

void GameConfig::incGameModeType() {
	gameMode++;
	if (gameMode > 2) {
		gameMode = 0;
	}
}
void GameConfig::decGameModeType() {
	gameMode--;
	if (gameMode < 0) {
		gameMode = 2;
	}
}
int GameConfig::getGameModeType() {
	return gameMode;
}

/*************************
 *
 * CONFIGURATION SERVER
 *
 *************************/
void GameConfig::incNbClientServer() {
	nbClientServer++;
	if (nbClientServer > nbClientMax) {
		nbClientServer = 1;
	}
}

void GameConfig::decNbClientServer() {
	nbClientServer--;
	if (nbClientServer < 1) {
		nbClientServer = nbClientMax;
	}
}

int GameConfig::getNbClientServer() {
	return nbClientServer;
}

/*************************
 *
 * CONFIGURATION CLIENT
 *
 *************************/
void GameConfig::incNbPlayerOfClient() {
	nbPlayerOfClient++;
	if (nbPlayerOfClient > nbClientMax) {
		nbPlayerOfClient = 1;
	}
}

void GameConfig::decNbPlayerOfClient() {
	nbPlayerOfClient--;
	if (nbPlayerOfClient < 1) {
		nbPlayerOfClient = nbClientMax;
	}
}

int GameConfig::getNbPlayerOfClient() {
	return nbPlayerOfClient;
}

void GameConfig::incIpValue(int idx, int val) {
	ipTab[idx] += val;
	if (ipTab[idx] > 255) {
		ipTab[idx] = 255;
	}
	sprintf(ipString, "%i.%i.%i.%i", ipTab[0], ipTab[1], ipTab[2], ipTab[3]);
}

void GameConfig::decIpValue(int idx, int val) {
	ipTab[idx] -= val;
	if (ipTab[idx] < 0) {
		ipTab[idx] = 0;
	}
	sprintf(ipString, "%i.%i.%i.%i", ipTab[0], ipTab[1], ipTab[2], ipTab[3]);
}

char * GameConfig::getIpString() {
	sprintf(ipString, "%i.%i.%i.%i", ipTab[0], ipTab[1], ipTab[2], ipTab[3]);
	return ipString;
}

char * GameConfig::getIpStringForMenu() {
	sprintf(ipString, "%3i.%3i.%3i.%3i", ipTab[0], ipTab[1], ipTab[2], ipTab[3]);
	for (int i = 0; i < 16; i++) {
		if (ipString[i] == ' ') {
			ipString[i] = '0';
		}
	}

	return ipString;
}

void GameConfig::incPortValue(int val) {
	port += val;
	if (port > 65535) {
		port = 65535;
	}
}

void GameConfig::decPortValue(int val) {
	port -= val;
	if (port < 0) {
		port = 0;
	}
}

int GameConfig::getPortValue() {
	return port;
}

char * GameConfig::getPortValueForMenu() {
	sprintf(portString, "%5i", port);
	for (int i = 0; i < 5; i++) {
		if (portString[i] == ' ') {
			portString[i] = '0';
		}
	}

	return portString;
}

bool GameConfig::getAcceptClient() {
	return acceptClient;
}

void GameConfig::setAcceptClient(bool accept) {
	acceptClient = accept;
}

void GameConfig::addNetPlayer(int num) {
	numberOfNetPlayer += num;
}
void GameConfig::removeNetPlayer(int num) {
	numberOfNetPlayer -= num;
}
int GameConfig::getNbNetPlayer() {
	return numberOfNetPlayer;
}

void GameConfig::generateNetPlayerConfiguration() {
	if (gameMode == LOCAL) {
		for (int i = 1; i < nbPlayer; i++) {
			playerType[i] = CPU;
		}
	} else {
		fprintf(stderr, "generate player net : %i", numberOfNetPlayer);
		int n = 0;
		for (int i = 1; i < nbPlayer; i++) {
			if (n >= numberOfNetPlayer) {
				playerType[i] = CPU;
			} else {
				playerType[i] = NET;
				n++;
			}
		}
	}
}
bool GameConfig::netPlayerAllSet() {
	int nb = 0;
	for (int i = 0; i < nbPlayer; i++) {
		if (playerType[i] == NET) {
			nb++;
		}
	}
	if (nb == numberOfNetPlayer) {
		return true;
	} else {
		return false;
	}
}

void GameConfig::resetNumberNetPlayer() {
	numberOfNetPlayer = 0;
	for (int i = 1; i < nbPlayer; i++) {
		playerType[i] = CPU;
	}
}

void GameConfig::setAdresseOfKeystateOverNet(unsigned short * in_keystate_over_net) {
	this->in_keystate_over_net = in_keystate_over_net;
}

void GameConfig::setKeyPressedForNetPlayer(int player, unsigned short val) {
	in_keystate_over_net[player] = val;
}

void GameConfig::setAdresseOfKeystate(unsigned short * in_keystate) {
	this->in_keystate = in_keystate;
}

unsigned short GameConfig::getKeystate(int player) {
	return in_keystate[player];
}

void GameConfig::incNbReservedPlayerServer(){
	nbReservedPlayerServer++;
	if(nbReservedPlayerServer >16){
		nbReservedPlayerServer =1;
	}

}
void GameConfig::decNbReservedPlayerServer(){
	nbReservedPlayerServer--;
	if(nbReservedPlayerServer < 1){
		nbReservedPlayerServer = 16;
	}
}
int GameConfig::getNbReservedPlayerServer(){
	return nbReservedPlayerServer;
}
