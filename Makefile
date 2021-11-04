UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
output: game
	g++ -std=c++17 main2.cpp \
	modules/buffer.cpp \
	modules/camera.cpp \
	modules/collisions.cpp \
	modules/entity.cpp \
	modules/events.cpp \
	modules/game.cpp \
	modules/hero.cpp \
	modules/maps.cpp \
	modules/mouse.cpp \
	modules/quads.cpp \
	modules/shaders.cpp \
	modules/textures.cpp \
	modules/timer.cpp \
	modules/utils.cpp -o game -lSDL2 -lGLEW -lfreetype -framework OpenGL
game: main2.cpp
clean: rm *.o
endif
ifeq ($(UNAME),Linux)
output: game
	g++ -std=c++17 main2.cpp \
	modules/buffer.cpp \
	modules/camera.cpp \
	modules/collisions.cpp \
	modules/entity.cpp \
	modules/events.cpp \
	modules/game.cpp \
	modules/hero.cpp \
	modules/maps.cpp \
	modules/mouse.cpp \
	modules/quads.cpp \
	modules/shaders.cpp \
	modules/textures.cpp \
	modules/timer.cpp \
	modules/utils.cpp -o game -lSDL2 -lGL -lGLEW -lfreetype
game: main2.cpp
clean: rm *.o
endif