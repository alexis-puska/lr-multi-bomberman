#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include "utils/Sprite.h"
#include "utils/LevelService.h"

#define nbBombeMax	 			6
#define nbBombeMin	 			1
#define nbBombedefault 			2
#define bombeStrenghtMax		6
#define bombeStrenghtMin		1
#define bombeStrenghtDefault	2
#define nbPlayer				16
#define nbTypeBonus				13
#define nbMaxBonusTotal			150
#define nbMaxBonusCat			50
#define timeOfGameMax			5
#define timeOfGameMin			2
#define timeOfGameDefault		3
#define timeOfGameInf			-1

#ifndef __MYCLASS_GAMECONFIG
#define __MYCLASS_GAMECONFIG

class GameConfig {

	public:
		GameConfig();
		~GameConfig();
		static GameConfig& Instance();
		void resetConfigEndGame();
		
		void setLevel(int n);
		int getLevel();
		void setVariante(int n);
		int getVariante();
		void setBombe(int n)
		int getBombe();
		void setStrengthBombe(int n);
		int getStrenghtBombe();
		void incBonus(int idx);
		void inc5Bonus(int idx);
		void decBonus(int idx);
		void dec5Bonus(int idx);
		int sumTotalBonus();
		int getBonus(int idx);
		void switchCustomBonus();
		bool isCustomBonus();
		void switchSuddenDeathMode();
		bool isSuddentDeathMode();
		void switchBadBomberMode();
		bool isBadBomberMode();
		void setIALevel(int n);
		int getIALevel();
		void setTimeOfGame(int n);
		int getTimeOfGame();
		void setNbPlayerInGame(int n);
		int getNbPlayerInGame();
		void setPlayerType(int idx, int val);
		int getPlayerType(int idx);
		void setPlayerColor(int idx, int val);
		int getPlayerColor(int idx);
		void setPlayerSpriteType(int idx, int val);
		int setPlayerSpriteType(int idx);
		void setPlayerScore(int idx, int val);
		int getPlayerScore(int idx);
		void resetPlayerScore();
		void getPlayerStatus(int idx, int val);
		int setPlayerStatus(int idx);
		void resetPlayerStatus();
		
	private:
		GameConfig& operator=(const GameConfig&);
		GameConfig(const GameConfig&);
		static GameConfig m_instance;
	
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
