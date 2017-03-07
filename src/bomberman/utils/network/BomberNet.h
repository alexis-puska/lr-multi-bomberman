#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_net.h>
#else
#include <SDL2_net/SDL_net.h>
#endif

#ifndef __MYCLASS_BOMBERNET
#define __MYCLASS_BOMBERNET

#define MAKE_NUM(A, B, C, D)	(((A+B)<<8)|(C+D))
#define GAME_SCROLLBACK		512
#define GAME_PROMPT		"> "
#define GAME_PACKETSIZE		256
#define GAME_PORT			7777
#define GAME_HELLO		0
#define GAME_HELLO_PORT		1
#define GAME_HELLO_NLEN		GAME_HELLO_PORT+2
#define GAME_HELLO_NAME		GAME_HELLO_NLEN+1
#define GAME_ADD			1
#define GAME_ADD_SLOT		1
#define GAME_ADD_HOST		GAME_ADD_SLOT+1
#define GAME_ADD_PORT		GAME_ADD_HOST+4
#define GAME_ADD_NLEN		GAME_ADD_PORT+2
#define GAME_ADD_NAME		GAME_ADD_NLEN+1
#define GAME_DEL			2
#define GAME_DEL_SLOT		1
#define GAME_DEL_LEN		GAME_DEL_SLOT+1
#define GAME_BYE			255
#define GAME_BYE_LEN		1
#define GAME_ID 			5
#define GAME_ID_SLOT 		1
#define GAME_ID_LEN 		2
#define GAME_MAXPEOPLE		10

class BomberNet {

	public:
		BomberNet();
		~BomberNet();
		static BomberNet& Instance();
		static TCPsocket servsock;
		static SDLNet_SocketSet socketset;

		void createTcpServer();
		void startServer();
		void stopServer();
		void sendLine();

	private:
		BomberNet& operator=(const BomberNet&);
		BomberNet(const BomberNet&);
		static BomberNet m_instance;

		//server thread
		static bool alive;
		static int net_thread_main(void *data);
		SDL_Thread *net_thread;

		void initSDLNet();
		void cleanup();
		bool isAlive();
		void allocateSockets();
		void createServerSocket();
		void addInactiveSocket(int which, TCPsocket newsock);
		void roomFull(TCPsocket newsock);
		void HandleServer(void);
		void HandleClient(int which);
		void deleteConnection(int which);
		void findInactivePersonSlot(int &which);
		void notifyAllConnectionClosed(char data[], int which);
		void SendNew(int which, int i);
		void SendID(int which);

};
#endif
