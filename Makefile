UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
output: game
	g++ -std=c++11 main.cpp -o game -lSDL2 -lGLEW -framework OpenGL
game: main.cpp
clean: rm *.o
endif
ifeq ($(UNAME),Linux)
output: game
	g++ -std=c++11 main.cpp -o game -lSDL2 -lGL -lGLEW
game: main.cpp
clean: rm *.o
endif