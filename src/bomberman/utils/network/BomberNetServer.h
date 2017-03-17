#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_net.h>
#else
#include <SDL2_net/SDL_net.h>
#endif
#include <map>
#include "../GameConfig.h"

#ifndef __MYCLASS_BOMBERNETSERVER
#define __MYCLASS_BOMBERNETSERVER

#define GAME_BYE			255
#define GAME_MAXPEOPLE		15

class BomberNetServer {
	public:
		static BomberNetServer& Instance();
		static TCPsocket servsock;
		static SDLNet_SocketSet socketset;
		static int nbClientConnected;
		static int requestNumber;
		static std::map<int, int> connexionHuman;

		BomberNetServer();
		~BomberNetServer();

		bool createTcpServer();
		void startServer();
		void stopServer();
		int getNbClientConnected();

		void sendServerFull(TCPsocket newsock);
		void sendServerInGame(TCPsocket newsock);
		void sendAcknoledgementOfClientPlayer(int which);
		void sendErrorSlotAvailable(int which);
		void linkKeystate();

		void sendNbConnected();
		void sendChangeScreenCommand(int screen);
		void sendPlayerType();
		void sendSpriteType();
		void sendGameOption();
		void sendLevelInfo();

	private:
		static BomberNetServer m_instance;

		//DRAW REQUEST BUFFER
		char buffer[1024];
		int bufferLenght;
		int bufferPosition;
		int bufferElement;
		void concatBuffer(char * src, int length);
		void initBuffer();
		void sendBuffer();


		//server thread
		static bool alive;
		static int net_thread_main(void *data);
		SDL_Thread *net_thread;

		BomberNetServer& operator=(const BomberNetServer&);
		BomberNetServer(const BomberNetServer&);

		bool isAlive();
		void initSDLNet();
		void cleanup();
		void allocateSockets();
		bool createServerSocket();
		void addInactiveSocket(int which, TCPsocket newsock);
		void HandleServer(void);
		void HandleClient(int which);
		void deleteConnection(int which);
		void findInactivePersonSlot(int &which);
		void sendSlotAvailable(int which);
		void decode(char data[1024], int which);
};
#endif
