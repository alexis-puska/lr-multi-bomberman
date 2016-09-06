Lr-Multi-Bomberman
===============================
Bomberman oriented multiplayer (16 players, max of retroarch)
Library core for Retroarch emulator.


===============================
SDL2 Library installation : 
- Linux / Ubuntu



- Raspberry pi 2



- mac os X


===============================
Compilation : 

Unix / mac os X : 
make clean -f Makefile.libretro
make -f Makefile.libretro


Raspberry pi 2/3
edit the file : Makefile.libretro
uncommente the line : #platform = rpi
add the version of your raspberry pi
2 for the raspberry pi 2
3 for the raspberry pi 3

```
make clean -f Makefile.libretro
make -f Makefile.libretro
```
