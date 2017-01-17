#include "GameConfig.h"

GameConfig GameConfig::m_instance = GameConfig();

GameConfig& GameConfig::Instance() {
	return m_instance;
}

GameConfig::GameConfig() {
	level = 0;
	variante = 0;
	nbBombe = nbBombedefault;
	strengthBombe = bombeStrenghtDefault;
	
	suddenDeathMode = false;
	badBomberMode = false;
	IALevel = 1;
	timeOfGame = timeOfGameDefault;
	
	nbplayerInGame = 0;
	for(int i = 0 ; i < nbBonus ; i++){
		bonus[i] = 0;
	}
	for(int i = 0 ; i < nbPlayer ; i++){
		playerType[i] = 0;
		playerColor[i] = 0;
		playerSpriteType[i] = 0;
	}
	resetPlayerScore();
}

GameConfig::~GameConfig() {
}


	
void GameConfig::incLevel(){
	level++;
	if(n > LevelService::getNumberOfLevels()){
		level = 0;
	}
}

void GameConfig::decLevel(){
	level--;
	if(n < 0){
		level = LevelService::getNumberOfLevels();
	}
}
	
int GameConfig::getLevel(){
	return level;
}
	
void GameConfig::incVariante(int n){
	variante++;
	if( variante > LevelService::getLevel()->getVariantesSize()){
		variante = 0;
	}
}

void GameConfig::decVariante(int n){
	variante--;
	if(variante < 0){
		variante = LevelService::getLevel()->getVariantesSize();
	}
}
	
int GameConfig::getVariante(){
	return variante;
}

void GameConfig::setBombe(int n){
	if(n > nbBombeMax){
		bombe = nbBombeMin;
	}else if(n < nbBombeMin){
		bombe = nbBombeMax;
	}else{
		bombe = n;
	}
}

int GameConfig::getBombe(){
	return bombe;
}

void GameConfig::setStrengthBombe(int n){
	if(n > bombeStrenghtMax){
		strengthBombe = bombeStrenghtMin;
	}else if(n < bombeStrenghtMin){
		strengthBombe = bombeStrenghtMax;
	}else{
		strengthBombe = n;
	}
}

int GameConfig::getStrenghtBombe(){
	return strengthBombe;
}

void GameConfig::incBonus(int idx){
	if(sumTotalBonus() + 1) > nbMaxBonusTotal){
		return;
	}
	bonus[idx]++;
	if(bonus[idx] > nbMaxBonusCat){
		bonus[idx] = nbMaxBonusCat;
	}
}

void GameConfig::inc5Bonus(int idx){
	if(sumTotalBonus() + 5) > nbMaxBonusTotal){
		return;
	}
	bonus[idx] = bonus[idx] + 5;
	if(bonus[idx] > nbMaxBonusCat){
		bonus[idx] = nbMaxBonusCat;
	}
}

void GameConfig::decBonus(int idx){
	bonus[idx]--;
	if(bonus[idx] < 0 ){
		bonus[idx] = 0;
	}
}

void GameConfig::dec5Bonus(int idx){
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
}

bool GameConfig::isBadBomberMode(){
	return badBomberMode;
}

void GameConfig::setIALevel(int n){
	if(n > bombeStrenghtMax){
		IALevel = bombeStrenghtMin;
	}else if(n < bombeStrenghtMin){
		IALevel = bombeStrenghtMax;
	}else{
		IALevel = n;
	}
}

int GameConfig::getIALevel(){
	return IALevel;
}

void GameConfig::incTimeOfGame(){
	timeOfGame++;
	if(timeOfGame > timeOfGameMax){
		timeOfGame = timeOfGameInf;
	}else if(timeOfGame == (timeOfGameInf+1)){
		timeOfGame = timeOfGameMin;
	}
}
void GameConfig::decTimeOfGame(){
	timeOfGame--;
	if(timeOfGame < timeOfGameMin){
		timeOfGame = timeOfGameInf;
	}else if(timeOfGame == (timeOfGameInf-1)){
		timeOfGame = timeOfGameMax;
	}
}

int GameConfig::getTimeOfGame(){
	return timeOfGame;
}

void GameConfig::setNbPlayerInGame(int n){
	nbplayerInGame = n;
}

int GameConfig::getNbPlayerInGame(){
	return nbplayerInGame;
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

void GameConfig::setPlayerSpriteType(int idx, int val){
	playerSpriteType[idx] = val;
}

int GameConfig::setPlayerSpriteType(int idx){
	return playerSpriteType[idx];
}

void GameConfig::setPlayerScore(int idx, int val){
	playerScore[idx] = val;
}

int GameConfig::getPlayerScore(int idx){
	return playerScore[idx];
}

void GameConfig::resetPlayerScore(){
	for(int i = 0 ; i < nbPlayer ; i++){
		playerScore[i] = 0;
	}
}

void GameConfig::getPlayerStatus(int idx, int val){
	playerStatus[idx] = val;
}

int GameConfig::setPlayerStatus(int idx){
	return playerStatus[idx];
}

void GameConfig::resetPlayerStatus(){
	for(int i = 0 ; i < nbPlayer ; i++){
		playerStatus[i] = 0;
	}
}