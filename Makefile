# Makefile for Lr-Multi-Bomberman


# default stuff goes here, so that config can override
TARGET ?= lr-multi-bomberman
CFLAGS += -Wall -ggdb -Iinclude -ffast-math
ifndef DEBUG
CFLAGS += -O2 -DNDEBUG
endif
CXXFLAGS += $(CFLAGS)
#DRC_DBG = 1
#PCNT = 1


all: target_


CC_LINK ?= $(CC)
CC_AS ?= $(CC)
LDFLAGS += $(MAIN_LDFLAGS)
EXTRA_LDFLAGS ?= -Wl,-Map=$@.map
LDLIBS += $(MAIN_LDLIBS)
ifdef PCNT
CFLAGS += -DPCNT
endif


#OBJECT TO COMPILE
OBJS += src/libretro/libretro.o
OBJS += src/bomberman/Bomberman.o
OBJS += src/bomberman/utils/network/BomberNetServer.o
OBJS += src/bomberman/utils/network/BomberNetClient.o
OBJS += src/bomberman/MyWrapper.o
OBJS += src/bomberman/Grid.o
OBJS += src/bomberman/Hole.o
OBJS += src/bomberman/Mine.o
OBJS += src/bomberman/Teleporter.o
OBJS += src/bomberman/utils/RailSwitch.o
OBJS += src/bomberman/trolley/Trolley.o
OBJS += src/bomberman/trolley/Rail.o
OBJS += src/bomberman/trolley/Button.o
OBJS += src/bomberman/Game.o
OBJS += src/bomberman/ClientViewer.o
OBJS += src/bomberman/Player.o
OBJS += src/bomberman/Bombe.o
OBJS += src/bomberman/animation/Explosion.o
OBJS += src/bomberman/animation/BurnWall.o
OBJS += src/bomberman/animation/BurnLouis.o
OBJS += src/bomberman/animation/Cursor.o
OBJS += src/bomberman/animation/SuddenDeathAnimation.o
OBJS += src/bomberman/animation/BurnBonus.o
OBJS += src/bomberman/animation/PopBonus.o
OBJS += src/bomberman/ia/Brain.o
OBJS += src/bomberman/ia/AStarCell.o
OBJS += src/bomberman/ia/AStar.o
OBJS += src/bomberman/ia/BFS.o
OBJS += src/bomberman/utils/Sprite.o
OBJS += src/bomberman/utils/Sound.o
OBJS += src/bomberman/utils/LevelService.o
OBJS += src/bomberman/utils/Level.o
OBJS += src/bomberman/utils/LevelVariante.o
OBJS += src/bomberman/utils/GameConfig.o



#COMPILATION
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c -o $@ $<


target_: $(TARGET)


#LINK
$(TARGET): $(OBJS)	
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS) $(EXTRA_LDFLAGS)


#CLEAN
clean: $(PLAT_CLEAN) 
	$(RM) $(TARGET) $(OBJS) $(TARGET).map