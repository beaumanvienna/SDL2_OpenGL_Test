# SDL2_OpenGL_Test
A simple test to see if the SDL2 renderer and bare OpenGL commands work in the same program

prerequisites for Ubuntu 18.04
sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libgl-dev libgl1

compile with
g++ main.cpp -o main -I/usr/include/SDL2 -I/usr/include/GL -lSDL2 -lGL -lSDL2_image
