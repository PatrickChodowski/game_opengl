UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
output: game
	g++ -std=c++17 main.cpp \
	modules/buffer.cpp \
	modules/camera.cpp \
	modules/collisions.cpp \
	modules/entity.cpp \
	modules/events.cpp \
	modules/maps.cpp \
	modules/quad.cpp \
	modules/textures.cpp \
	modules/utils.cpp -o game -lSDL2 -lGLEW -lfreetype -framework OpenGL
game: main.cpp
clean: rm *.o
endif
ifeq ($(UNAME),Linux)
output: game
	g++ -std=c++17 main.cpp \
	modules/buffer.cpp \
	modules/camera.cpp \
	modules/collisions.cpp \
	modules/entity.cpp \
	modules/events.cpp \
	modules/maps.cpp \
	modules/quad.cpp \
	modules/textures.cpp \
	modules/utils.cpp -o game -lSDL2 -lGL -lGLEW -lfreetype
game: main.cpp
clean: rm *.o
endif