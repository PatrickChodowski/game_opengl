UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
output: game
	g++ -std=c++17 main.cpp -o game -lSDL2 -lGLEW -lfreetype -framework OpenGL
game: main.cpp
clean: rm *.o
endif
ifeq ($(UNAME),Linux)
output: game
#	g++ -std=c++17 main.cpp modules/entity.cpp modules/quad.cpp modules/collisions.cpp -o game -lSDL2 -lGL -lGLEW -lfreetype
	g++ -std=c++17 main.cpp modules/entity.cpp modules/quad.cpp modules/collisions.cpp -o game -lSDL2 -lGL -lGLEW -lfreetype
game: main.cpp
clean: rm *.o
endif