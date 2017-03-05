#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_net.h>
#else
#include <SDL2_net/SDL_net.h>
#endif


#ifndef __MYCLASS_BOMBERNET
#define __MYCLASS_BOMBERNET

class BomberNet {

	public:
		BomberNet();
		~BomberNet();
		static BomberNet& Instance();


	private:
		BomberNet& operator=(const BomberNet&);
		BomberNet(const BomberNet&);
		static BomberNet m_instance;

};
#endif
