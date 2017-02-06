#include "RailSwitch.h"

RailSwitch::RailSwitch(int prev, int next, int nextAlt){
	this->prevIndex = prev;
	this->nextIndex = next;
	this->nextIndexAlt = nextAlt;
}
RailSwitch::~RailSwitch(){
}
int RailSwitch::getPrevIndex(){
	return prevIndex;
}
int RailSwitch::getNextIndex(){
	return nextIndex;
}
int RailSwitch::getNextIndexAlt(){
	return nextIndexAlt;
}
