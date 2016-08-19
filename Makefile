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

-include Makefile.local

CC_LINK ?= $(CC)
CC_AS ?= $(CC)
LDFLAGS += $(MAIN_LDFLAGS)
EXTRA_LDFLAGS ?= -Wl,-Map=$@.map
LDLIBS += $(MAIN_LDLIBS)
ifdef PCNT
CFLAGS += -DPCNT
endif

# misc
OBJS += src/libretro/libretro.o
OBJS += src/bomberman/Bomberman.o
OBJS += src/bomberman/MyWrapper.o


CFLAGS += -DHAVE_LIBRETRO


%.o: %.S
	$(CC_AS) $(CFLAGS) -c $^ -o $@



%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c -o $@ $<



target_: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJS)
else
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS) $(EXTRA_LDFLAGS)
endif



clean: $(PLAT_CLEAN) 
	$(RM) $(TARGET) $(OBJS) $(TARGET).map

# ----------- release -----------

VER ?= $(shell git describe HEAD)

ifeq "$(PLATFORM)" "generic"
OUT = pcsx_rearmed_$(VER)

rel: pcsx $(PLUGINS) \
		frontend/pandora/skin readme.txt COPYING
	rm -rf $(OUT)
	mkdir -p $(OUT)/plugins
	mkdir -p $(OUT)/bios
	cp -r $^ $(OUT)/
	mv $(OUT)/*.so* $(OUT)/plugins/
	zip -9 -r $(OUT).zip $(OUT)
endif
