#include "Cursor.h"
#include "Sprite.h"

#define frameFrequence 100

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
}

Cursor::~Cursor() {
	stopAnimation();
}

SDL_Surface * Cursor::getCurrentFrame() {
	return Sprite::Instance().getCursor(position);
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
