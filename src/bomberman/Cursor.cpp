#include "Cursor.h"

#define frameFrequence 100
#define spriteSize 16
#define nbFrame 4

const static char *cursorSprite = "./resources/sprite/other/Cursor.png";

static int animateCursor(void* data) {
	Cursor *cursor = ((Cursor *) data);
	while (cursor->isAlive()) {
		SDL_Delay(frameFrequence);
		cursor->setPosition(cursor->getPosition() + 1);
		if (cursor->getPosition() > 3) {
			cursor->setPosition(0);
		}
	}
	return 0;
}

Cursor::Cursor() {
	position = 0;

	isThreadAlive = false;
	//Initialise all image of the cursor

	//declarativ color mask, used for create a RGB surface
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;

	cursor = new SDL_Surface*[4];

	SDL_Surface *cursorBuffer = IMG_Load(cursorSprite);
	SDL_Rect src;
	SDL_Rect dst;

	dst.x = 0;
	dst.y = 0;
	dst.w = spriteSize;
	dst.h = spriteSize;
	for (int i = 0; i < nbFrame; i++) {
		src.x = i * spriteSize;
		src.y = 0;
		src.w = spriteSize;
		src.h = spriteSize;
		cursor[i] = SDL_CreateRGBSurface(0, spriteSize, spriteSize, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(cursorBuffer, &src, cursor[i], &dst);
	}
	SDL_FreeSurface(cursorBuffer);
}

Cursor::~Cursor() {
	stopAnimation();
	for (int i = 0; i < nbFrame; i++) {
		SDL_FreeSurface (cursor[i]);
	}
	free (cursor);
}

SDL_Surface * Cursor::getCurrentFrame() {
	return cursor[position];
}

bool Cursor::isAlive() {
	return isThreadAlive;
}

int Cursor::getPosition() {
	return position;
}

void Cursor::setPosition(int newPosition) {
	position = newPosition;
}

void Cursor::startAnimation() {
	//start the thread for animate the cursor
	if (!isThreadAlive) {
		isThreadAlive = true;
		cursorThread = SDL_CreateThread(animateCursor, "cursorThread", this);
	}
}

void Cursor::stopAnimation() {
	//stop the thread for animate the cursor
	if (isThreadAlive) {
		isThreadAlive = false;
		int treadResult;
		SDL_WaitThread(cursorThread, &treadResult);
	}
}
