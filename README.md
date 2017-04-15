**Lr-Multi-Bomberman**
===============================

> Bomberman oriented multiplayer (16 players, max of retroarch) Library
> core for Retroarch emulator.


===============================
**SDL2 Library installation :** 

- Linux / Ubuntu
```
sudo apt-get install -y make libsdl2-dev 
sudo apt-get install -y make libsdl2-image-dev 
sudo apt-get install -y make libsdl2-ttf-dev 
sudo apt-get install -y make libsdl2-mixer-dev 
sudo apt-get install -y make libsdl2-net-dev 

- Linux / Fedora (with RPM Fusion)
sudo dnf install SDL2-devel
sudo dnf install SDL2_mixer-devel
sudo dnf install SDL2_ttf-devel
sudo dnf install SDL2_image-devel
sudo dnf install SDL2_net-devel
```


- Raspberry pi 2/3 : 

```
connection SSH : 
reset ssh token : ssh-keygen -R "ip";

reset smb password : smbpasswd -a pi
sudo apt-get update
sudo apt-get install -y make libsdl2-dev libsdl2-image-dev libsdl2-net-dev libsdl2-ttf-dev libsdl2-mixer-dev libXinerama-dev libXi-dev libXrandr-dev libXss-dev libXCursor-dev
```


- mac os X : 
```
Download the SDL2 library
https://www.libsdl.org/release/SDL2-2.0.4.dmg
https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.dmg
https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.1.dmg
https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.1.dmg
https://www.libsdl.org/projects/SDL_net/release/SDL2_net-2.0.1.dmg
unzip and install the SDL2_image.framework, SDL2_ttf.framework, SDL2.framework inside the folder : /Library/Frameworks/
```



===============================
**Compilation :**

Unix / mac os X : 
```
make clean -f Makefile.libretro
make -f Makefile.libretro
```

Raspberry pi 2/3: 

Change your swapFile default size, since the sprite, sound, musique, image is encode un binary in the code we need to have more swap to compile 
- sudo nano /etc/dphys-swapfile
The default value in Raspbian is:
```
CONF_SWAPSIZE=100
```
change to 
```
CONF_SWAPSIZE=200
```

- edit the file : Makefile.libretro
- uncommente the line : #platform = rpi
- add the version of your raspberry pi
- 2 for the raspberry pi 2
- 3 for the raspberry pi 3

===============================
**Launch :**
```
	/PathToRetroarch/retroarch -L lr_multi_bomberman_libretro.so
```

===============================
**Installation on retropie :**

add this xml fragment to es_systems.cfg file in /etc/emulationstation/es_systems.cfg
```
  <system>
    <name>bomberman</name>
    <fullname>bomberman</fullname>
    <plateform>bomberman</plateform>
    <path>/home/pi/RetroPie/roms/lr-multi-bomberman</path>
    <command>sudo %ROM%</command>
    <!--<command>/opt/retropie/emulators/retroarch/bin/retroarch -L %ROM% --config /opt/retropie/configs/all/retroarch.cfg</command>-->
    <extension>.sh</extension>
    <theme>bomberman</theme>
  </system>
```


in a folder clone the project : 
```
cd /home/pi/RetroPie/roms
git clone https://github.com/alexis-puska/lr-multi-bomberman.git
cd lr-multi-bomberman
chmod +x Update.sh
chmod +x Compile\ pi.sh
chmod +x Bomberman.sh
./Compile\ pi.sh
cd emulationStationTheme
sudo cp -avr bomberman /etc/emulationstation/themes/carbon
sudo reboot
```

After this step lr-multi-bomberman is installed, compile, and integrated in emulation station on retropie distribution. If an update of code is made, you can just launch the UPDATE line in emulation station, COMPILE PI and launch the game with BOMBERMAN :) Enjoy !


===============================

**Next step :**

|description                                |wait|in progress|done|
|-------------------------------------------|:--:|:---------:|:--:|
|Louis type                                 |    |           |X   |
|level variante                             |    |           |X   |
|menu to set number of bonus                |    |           |X   |
|refactoring of code                        |    |           |X   |
|player underwater                          |    |           |X   |
|button	                                    |    |           |X   |
|rail                                       |    |           |X   |
|Holl in game                               |    |           |X   |
|Teleporter                                 |    |           |X   |
|Mine                                       |    |           |X   |
|Trolley                                    |    |           |X   |
|make level for trolley	                    |    |           |X   |
|Game type bad bomber                       |    |           |X   |
|other level design                         |    |           |X   |
|Net (multiplayer over internet)            |    |           |X   |
|Artificial interlligence                   |    |X          |    |
|Power(glove)                               |X   |           |    |

convoyeur (3 * 8)
bouton (2)
rail (8)
fleche (4)
fleche rotative (4)
bloc level 3 (1)
trou (3)
mine sous marine (4)
sable mouvant (4)
teleporter (3)





sudo apt-get install mxe-x86-64-w64-mingw32.static-sdl2 mxe-x86-64-w64-mingw32.static-sdl2-image mxe-x86-64-w64-mingw32.static-sdl2-ttf mxe-x86-64-w64-mingw32.static-sdl2-mixer mxe-x86-64-w64-mingw32.static-sdl2-gfx mxe-x86-64-w64-mingw32.static-sdl2-net mxe-x86-64-w64-mingw32.static-gcc mxe-x86-64-w64-mingw32.static-mingw-w64

