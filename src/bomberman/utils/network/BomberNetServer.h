#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_net.h>
#else
#include <SDL2_net/SDL_net.h>
#endif
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

		BomberNetServer();
		~BomberNetServer();

		bool createTcpServer();
		void startServer();
		void stopServer();
		void sendLine();
		int getNbClientConnected();

	private:
		static BomberNetServer m_instance;

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
		void roomFull(TCPsocket newsock);
		void serverAllReadyInGame(TCPsocket newsock);
		void HandleServer(void);
		void HandleClient(int which);
		void deleteConnection(int which);
		void findInactivePersonSlot(int &which);
};
#endif
