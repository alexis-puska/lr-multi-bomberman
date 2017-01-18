#include "GameConfig.h"

GameConfig GameConfig::m_instance = GameConfig();

GameConfig& GameConfig::Instance() {
	return m_instance;
}

GameConfig::GameConfig() {
	srand (time(NULL));
	level = 0;
	variante = 0;
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	suddenDeathMode = false;
	badBomberMode = false;
	IALevel = 1;
	timeOfGame = timeOfGameDefault;
	
	nbplayerInGame = 0;
	for(int i = 0 ; i < nbTypeBonus ; i++){
		bonus[i] = 0;
	}
	for(int i = 0 ; i < nbPlayer ; i++){
		playerType[i] = 1;
		playerColor[i] = 0;
		playerSpriteType[i] = 0;
	}
	playerType[0] = 0;
	resetPlayerScore();
	copyLevelBonus();
}

GameConfig::~GameConfig() {
}


	
void GameConfig::incLevel(){
	level++;
	if(level > LevelService::Instance().getNumberOfLevels() - 1){
		level = 0;
	}
	if(variante > LevelService::Instance().getLevel(level)->getVariantesSize()-1){
		variante = LevelService::Instance().getLevel(level)->getVariantesSize()-1;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}

void GameConfig::decLevel(){
	level--;
	if(level < 0){
		level = LevelService::Instance().getNumberOfLevels() - 1;
	}
	if(variante > LevelService::Instance().getLevel(level)->getVariantesSize()-1){
		variante = LevelService::Instance().getLevel(level)->getVariantesSize()-1;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}
	
int GameConfig::getLevel(){
	return level;
}
	
void GameConfig::incVariante(){
	variante++;
	if( variante > LevelService::Instance().getLevel(level)->getVariantesSize() - 1){
		variante = 0;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}

void GameConfig::decVariante(){
	variante--;
	if(variante < 0){
		variante = LevelService::Instance().getLevel(level)->getVariantesSize() - 1;
	}
	nbBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getNbBombe();
	strengthBombe = LevelService::Instance().getLevel(level)->getVariantes(variante)->getFireStrenght();
	copyLevelBonus();
}
	
int GameConfig::getVariante(){
	return variante;
}

void GameConfig::incBombe(){
	nbBombe++;
	if(nbBombe > nbBombeMax){
		nbBombe = nbBombeMin;
	}
}

void GameConfig::decBombe(){
	nbBombe--;
	if(nbBombe < nbBombeMin){
		nbBombe = nbBombeMax;
	}
}

int GameConfig::getBombe(){
	return nbBombe;
}

void GameConfig::incStrengthBombe(){
	strengthBombe++;
	if(strengthBombe > bombeStrenghtMax){
		strengthBombe = bombeStrenghtMin;
	}
}

void GameConfig::decStrengthBombe(){
	strengthBombe--;
	if(strengthBombe < bombeStrenghtMin){
		strengthBombe = bombeStrenghtMax;
	}
}

int GameConfig::getStrenghtBombe(){
	return strengthBombe;
}

void GameConfig::incBonus(int idx){
	customBonus = true;
	if((sumTotalBonus() + 1) > nbMaxBonusTotal){
		return;
	}
	bonus[idx]++;
	if(bonus[idx] > nbMaxBonusCat){
		bonus[idx] = nbMaxBonusCat;
	}
}

void GameConfig::inc5Bonus(int idx){
	customBonus = true;
	if((sumTotalBonus() + 5) > nbMaxBonusTotal){
		return;
	}
	bonus[idx] = bonus[idx] + 5;
	if(bonus[idx] > nbMaxBonusCat){
		bonus[idx] = nbMaxBonusCat;
	}
}

void GameConfig::decBonus(int idx){
	customBonus = true;
	bonus[idx]--;
	if(bonus[idx] < 0 ){
		bonus[idx] = 0;
	}
}

void GameConfig::dec5Bonus(int idx){
	customBonus = true;
	bonus[idx] = bonus[idx] - 5;
	if(bonus[idx] < 0 ){
		bonus[idx] = 0;
	}
}

int GameConfig::getBonus(int idx){
	return bonus[idx];
}

int GameConfig::sumTotalBonus(){
	int total = 0;
	for(int i = 0 ; i < nbTypeBonus ; i++){
		total += bonus[i];
	}
	return total;
}

void GameConfig::switchCustomBonus(){
	if(customBonus){
		customBonus = false;
		copyLevelBonus();
	}else{
		customBonus = true;
	}
}

bool GameConfig::isCustomBonus(){
	return customBonus;
}

void GameConfig::switchSuddenDeathMode(){
	if(suddenDeathMode){
		suddenDeathMode = false;
	}else{
		suddenDeathMode = true;
	}
}

bool GameConfig::isSuddentDeathMode(){
	return suddenDeathMode;
}

void GameConfig::switchBadBomberMode(){
	if(badBomberMode){
		badBomberMode = false;
	}else{
		badBomberMode = true;
	}
}

bool GameConfig::isBadBomberMode(){
	return badBomberMode;
}


void GameConfig::incIALevel(){
	IALevel++;
	if(IALevel > nbMaxIALevel){
		IALevel = 1;
	}
}

void GameConfig::decIALevel(){
	IALevel--;
	if(IALevel < 1){
		IALevel = nbMaxIALevel;
	}
}

int GameConfig::getIALevel(){
	return IALevel;
}

void GameConfig::incTimeOfGame(){
	if(timeOfGame == timeOfGameInf){
		timeOfGame = timeOfGameMin;
		return;
	}
	timeOfGame++;
	if(timeOfGame > timeOfGameMax){
		timeOfGame = timeOfGameInf;
	}
}
void GameConfig::decTimeOfGame(){
	if(timeOfGame == timeOfGameInf){
		timeOfGame = timeOfGameMax;
		return;
	}
	timeOfGame--;
	if(timeOfGame < timeOfGameMin){
		timeOfGame = timeOfGameInf;
	}
}

int GameConfig::getTimeOfGame(){
	return timeOfGame;
}

void GameConfig::setNbPlayerInGame(int n){
	nbplayerInGame = n;
}

int GameConfig::getNbPlayerInGame(){
	int nbplayerInGame = 0;
	for (int i = 0; i < 16; i++) {
		if (playerType[i] < 2) {
			nbplayerInGame++;
		}
	}
	return nbplayerInGame;
}

void GameConfig::incPlayerType(int idx){
	if(idx!=0){
		playerType[idx]++;
		if(playerType[idx] > 2){
			playerType[idx] = 0;
		}
	}
}

void GameConfig::decPlayerType(int idx){
	if(idx!=0){
		playerType[idx]--;
		if(playerType[idx] < 0){
			playerType[idx] = 2;
		}
	}
}

void GameConfig::setPlayerType(int idx, int val){
	playerType[idx] = val;
}

int GameConfig::getPlayerType(int idx){
	return playerType[idx];
}

void GameConfig::setPlayerColor(int idx, int val){
	playerColor[idx] = val;
}

int GameConfig::getPlayerColor(int idx){
	return playerColor[idx];
}

void GameConfig::generatePlayerSpriteTypeforCPU(){
	for (int i = 0; i < 16; i++) {
		if (playerType[i] == 1) {
			playerSpriteType[i] = (rand() % nbTypePlayer);
		}
	}
}
void GameConfig::generateColorPlayer(){
	for (int i = 0; i < 16; i++) {
		playerColor[i] = (rand() % nbColorPlayer);
	}
}

void GameConfig::incPlayerSpriteType(int idx){
	playerSpriteType[idx]++;
	if(playerSpriteType[idx] > nbTypePlayer){
		playerSpriteType[idx] = 0;
	}
}

void GameConfig::decPlayerSpriteType(int idx){
	playerSpriteType[idx]--;
	if(playerSpriteType[idx] < 0){
		playerSpriteType[idx] = nbTypePlayer;
	}
}

void GameConfig::setPlayerSpriteType(int idx, int val){
	playerSpriteType[idx] = val;
}

int GameConfig::getPlayerSpriteType(int idx){
	return playerSpriteType[idx];
}

void GameConfig::incPlayerScore(int idx){
	playerScore[idx]++;
}

int GameConfig::getPlayerScore(int idx){
	return playerScore[idx];
}

void GameConfig::resetPlayerScore(){
	for(int i = 0 ; i < nbPlayer ; i++){
		playerScore[i] = 0;
	}
}

void GameConfig::setPlayerDead(int idx){
	playerStatus[idx] = false;
}

void GameConfig::setPlayerAlive(int idx){
	playerStatus[idx] = true;
}

int GameConfig::isPlayerAlive(int idx){
	return playerStatus[idx];
}

void GameConfig::resetPlayerStatus(){
	for(int i = 0 ; i < nbPlayer ; i++){
		playerStatus[i] = 0;
	}
}

void GameConfig::copyLevelBonus(){
	for(int i = 0 ; i < nbTypeBonus ; i++){
		bonus[i] = LevelService::Instance().getLevel(level)->getVariantes(variante)->getBonus(i);
	}
}

void GameConfig::printPlayerConfiguration(){
	for (int i = 0; i < 16; i++) {
		fprintf(stderr, "%i %2it %2ic %2is %2isc %2ist\n", i, playerType[i], playerColor[i], playerSpriteType[i], playerScore[i], playerStatus[i]);
	}
}