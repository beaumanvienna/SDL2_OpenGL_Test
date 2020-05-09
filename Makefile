

all: main.cpp
	g++ main.cpp -o main -I/usr/include/SDL2 -I/usr/include/GL -lSDL2 -lGL -lSDL2_image

.PHONY: all
