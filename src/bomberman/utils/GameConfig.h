#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include "Sprite.h"
#include "LevelService.h"

#define nbBombeMax	 		6
#define nbBombeMin	 		1
#define nbBombedefault 			2
#define bombeStrenghtMax		6
#define bombeStrenghtMin		1
#define bombeStrenghtDefault		2
#define nbPlayer				16
#define nbMaxIALevel			5
#define nbTypeBonus			15
#define nbMaxBonusTotal			150
#define nbMaxBonusCat			50
#define timeOfGameMax			5
#define timeOfGameMin			2
#define timeOfGameDefault		3
#define timeOfGameInf			-1
#define nbClientMax			4

#define sizeX 35
#define sizeY 21

#ifndef __MYCLASS_GAMECONFIG
#define __MYCLASS_GAMECONFIG

enum playerTypeEnum {
	HUMAN = 0, CPU = 1, OFF = 2, NET = 3
};

enum gameModeEnum {
	LOCAL = 0, NET_SERVER = 1, NET_CLIENT = 2
};

class GameConfig {

	public:
		GameConfig();
		~GameConfig();
		static GameConfig& Instance();

		void incLevel();
		void decLevel();
		int getLevel();
		void incVariante();
		void decVariante();
		int getVariante();
		void incBombe();
		void decBombe();
		int getBombe();
		void incStrengthBombe();
		void decStrengthBombe();
		int getStrenghtBombe();
		void incBonus(int idx);
		void inc5Bonus(int idx);
		void decBonus(int idx);
		void dec5Bonus(int idx);
		int getBonus(int idx);
		int sumTotalBonus();
		void switchCustomBonus();
		bool isCustomBonus();
		void switchSuddenDeathMode();
		bool isSuddentDeathMode();
		void switchBadBomberMode();
		bool isBadBomberMode();
		void incIALevel();
		void decIALevel();
		int getIALevel();
		void incTimeOfGame();
		void decTimeOfGame();
		int getTimeOfGame();
		void setNbPlayerInGame(int n);
		int getNbPlayerInGame();
		void incPlayerType(int idx);
		void decPlayerType(int idx);
		void setPlayerType(int idx, int val);
		int getPlayerType(int idx);
		void setPlayerColor(int idx, int val);
		int getPlayerColor(int idx);
		void generatePlayerSpriteTypeforCPU();
		void generateColorPlayer();
		void incPlayerSpriteType(int idx);
		void decPlayerSpriteType(int idx);
		void setPlayerSpriteType(int idx, int val);
		int getPlayerSpriteType(int idx);
		void incPlayerScore(int idx);
		int getPlayerScore(int idx);
		void resetPlayerScore();
		void setPlayerDead(int idx);
		void setPlayerAlive(int idx);
		int isPlayerAlive(int idx);
		void resetPlayerStatus();
		void printPlayerConfiguration();

		void updatePlayerPosition(int player, float x, float y);
		void updatePlayerPosX(int player, float y);
		void updatePlayerPosY(int player, float y);
		float getPlayerPosX(int player);
		float getPlayerPosY(int player);
		int getPlayerIndex(int player);
		void resetPlayerCord();

		void incGameModeType();
		void decGameModeType();
		int getGameModeType();
		//server config
		void incNbClientServer();
		void decNbClientServer();
		int getNbClientServer();
		//client config
		void incNbPlayerOfClient();
		void decNbPlayerOfClient();
		int getNbPlayerOfClient();

		void incIpValue(int idx, int val);
		void decIpValue(int idx, int val);
		char * getIpString();
		char * getIpStringForMenu();

		void incPortValue(int val);
		void decPortValue(int val);
		int getPortValue();
		char * getPortValueForMenu();

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
		float playerCord[nbPlayer][2];
		int gameMode;
		int nbPlayerOfClient;
		int nbClientServer;
		int ipTab[4];
		int port;
		char ipString[17];
		char portString[6];

};
#endif
