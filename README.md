**Lr-Multi-Bomberman**
===============================

> Bomberman oriented multiplayer (16 players, max of retroarch) Library
> core for Retroarch emulator.


===============================
**SDL2 Library installation :** 

- Linux / Ubuntu
```
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-dev
```


- Raspberry pi 2/3 : 
```
sudo apt-get install -y make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```


- mac os X : 
```
Download the SDL2 library
https://www.libsdl.org/release/SDL2-2.0.4.dmg
https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.dmg
https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.1.dmg
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
**Next step :**

- Bonus (kick bombe, <del>flamme up, flamme max, bombe sup, louis, Gotta, roller</del>, block pass)
- Malus (diarhee(put bombe every time), constipation(can't put a bombe), fast bombe, slow bombe, slow player, fast player, switch player)
- Bombe type(<del>normal</del>, radio, power, rubber)
- Power(kick, glove)
- other level design (pirates, ring)
- Artificial interlligence
- Teleporter
- Game type (badBomber, sudden death)
- Louis burning animation
- Audio
- Net (multiplayer over interner)
