#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_net.h>
#else
#include <SDL2_net/SDL_net.h>
#endif
#include "../../ClientViewer.h"

#ifndef __MYCLASS_BOMBERNETCLIENT
#define __MYCLASS_BOMBERNETCLIENT

#define GAME_PORT			7777
#define GAME_BYE			255

class BomberNetClient {
	public:
		static BomberNetClient& Instance();
		static TCPsocket tcpsock;
		static SDLNet_SocketSet socketset;
		static int requestNumber;
		static int errorCode;
		static int errorValue;
		static ClientViewer * viewer;

		BomberNetClient();
		~BomberNetClient();

		void createTcpClient();
		int connectClient(SDL_Surface * vout_buf);
		void disconnectClient();

		void sendNbPlayerClient();
		static void sendKeystate();
		void sendDisconnection();
		bool isAlive();

	private:
		static BomberNetClient m_instance;

		//server thread
		static bool alive;
		static int net_thread_main(void *data);
		SDL_Thread *net_thread;

		SDL_Thread *keystate_thread;
		static bool keystateThreadAlive;
		//keystate of client player
		static bool keychange[16];
		static int nbPlayerOnThisClient;
		static unsigned short * keystate;
		static unsigned short previousPlayerKeystate[16];
		static int keystateThread(void *data);
		static bool checkKeystate();
		void startKeystateThread();
		void stopKeystateThread();




		BomberNetClient& operator=(const BomberNetClient&);
		BomberNetClient(const BomberNetClient&);
		void initSDLNet();
		void allocateSocket();
		void cleanup();
		int handleNet();
};
#endif
