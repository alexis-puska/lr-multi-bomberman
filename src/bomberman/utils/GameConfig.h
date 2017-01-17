#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include "Sprite.h"
#include "LevelService.h"

#define nbBombeMax	 			6
#define nbBombeMin	 			1
#define nbBombedefault 			2
#define bombeStrenghtMax		6
#define bombeStrenghtMin		1
#define bombeStrenghtDefault	2
#define nbPlayer				16
#define nbMaxIALevel				5
#define nbTypeBonus				13
#define nbMaxBonusTotal			150
#define nbMaxBonusCat			50
#define timeOfGameMax			5
#define timeOfGameMin			2
#define timeOfGameDefault		3
#define timeOfGameInf			-1

#ifndef __MYCLASS_GAMECONFIG
#define __MYCLASS_GAMECONFIG

enum playerTypeEnum {
	HUMAN = 0, CPU = 1, OFF = 2
};

class GameConfig {

	public:
		GameConfig();
		~GameConfig();
		static GameConfig& Instance();
		void resetConfigEndGame();
		
		void incLevel();
		void decLevel();
		void incVariante();
		void decVariante();
		void incBombe();
		void decBombe();
		void incStrengthBombe();
		void decStrengthBombe();
		void incBonus(int idx);
		void inc5Bonus(int idx);
		void decBonus(int idx);
		void dec5Bonus(int idx);
		void switchCustomBonus();
		void switchSuddenDeathMode();
		void switchBadBomberMode();
		void incIALevel();
		void decIALevel();
		void incTimeOfGame();
		void decTimeOfGame();
		void setNbPlayerInGame(int n);
		void incPlayerSpriteType(int idx);
		void decPlayerSpriteType(int idx);
		void incPlayerType(int idx);
		void decPlayerType(int idx);
		void setPlayerType(int idx, int val);
		void setPlayerColor(int idx, int val);
		void setPlayerSpriteType(int idx, int val);
		void setPlayerScore(int idx, int val);
		void resetPlayerScore();
		void getPlayerStatus(int idx, int val);
		void resetPlayerStatus();
		void generatePlayerSpriteTypeforCPU();

		int getLevel();
		int getVariante();
		int getBombe();
		int getStrenghtBombe();
		int sumTotalBonus();
		int getBonus(int idx);
		bool isCustomBonus();
		bool isSuddentDeathMode();
		bool isBadBomberMode();
		int getIALevel();
		int getTimeOfGame();
		int getNbPlayerInGame();
		int getPlayerType(int idx);
		int getPlayerColor(int idx);
		int getPlayerSpriteType(int idx);
		int getPlayerScore(int idx);
		int setPlayerStatus(int idx);
		
	private:
		GameConfig& operator=(const GameConfig&);
		GameConfig(const GameConfig&);
		static GameConfig m_instance;
		void copyLevelBonus();
		
		int level;
		int variante;
		int nbBombe;
		int strengthBombe;
		int bonus[nbTypeBonus];
		bool customBonus;
		bool suddenDeathMode;
		bool badBomberMode;
		int IALevel;
		int timeOfGame;
		int nbplayerInGame;
		int playerType[nbPlayer];
		int playerColor[nbPlayer];
		int playerSpriteType[nbPlayer];
		int playerScore[nbPlayer];
		int playerStatus[nbPlayer];
};
#endif
