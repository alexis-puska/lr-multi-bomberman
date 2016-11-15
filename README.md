**Lr-Multi-Bomberman**
===============================

> Bomberman oriented multiplayer (16 players, max of retroarch) Library
> core for Retroarch emulator.


===============================
**SDL2 Library installation :** 

- Linux / Ubuntu
```
sudo apt-get install libsdl2-image-mixer
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-dev
```


- Raspberry pi 2/3 : 
```
sudo apt-get install -y make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```


- mac os X : 
```
Download the SDL2 library
https://www.libsdl.org/release/SDL2-2.0.4.dmg
https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.dmg
https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.1.dmg
https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.1.dmg
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
Installation on retropie : 

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
- Artificial interlligence (current work)
- refactoring of code (change size of map for future pirate map, change the level structure definition for variante / bonus per level / teleporter)
- flamme un level 3 and 9 goes over the top and bottom of the map
- Teleporter
- other level design (pirates, ring)
- level variante
- menu to set number of bonus
- Game type bad bomber
- Louis burning animation
- louis type
- Net (multiplayer over interner)
- Power(glove)


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

