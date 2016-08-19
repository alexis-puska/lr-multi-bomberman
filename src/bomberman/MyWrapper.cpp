#include "Bomberman.h"
#include "MyWrapper.h"

extern "C" {
        Bomberman* newBomberman() {
                return new Bomberman();
        }

        void bomberman_getScreen(Bomberman* v) {
                v->getScreen();
        }

        void deleteBomberman(Bomberman* v) {
                delete v;
        }
}