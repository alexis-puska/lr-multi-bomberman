#ifndef __MYCLASS_RailSwitch
#define __MYCLASS_RailSwitch

class RailSwitch {
	public:
		RailSwitch(int prev, int next, int nextAlt);
		~RailSwitch();
		int getNextIndex();
		int getPrevIndex();
		int getNextIndexAlt();
	private:
		int nextIndex;
		int prevIndex;
		int nextIndexAlt;
};
#endif
