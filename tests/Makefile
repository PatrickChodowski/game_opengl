output: tests
	g++ -std=c++17 tests_main.cpp \
	../modules/anims.cpp \
	../modules/buffer.cpp \
	../modules/buttons.cpp \
	../modules/camera.cpp \
	../modules/collisions.cpp \
	../modules/debug.cpp \
	../modules/entity.cpp \
	../modules/events.cpp \
	../modules/fonts.cpp \
	../modules/game.cpp \
	../modules/hero.cpp \
	../modules/items.cpp \
	../modules/logger.cpp \
	../modules/maps.cpp \
	../modules/menu.cpp \
	../modules/mobs.cpp \
	../modules/mouse.cpp \
	../modules/navmesh.cpp \
	../modules/npcs.cpp \
	../modules/pathfinder.cpp \
	../modules/quads.cpp \
	../modules/saves.cpp \
	../modules/shaders.cpp \
	../modules/textures.cpp \
	../modules/travel.cpp \
	../modules/timer.cpp \
	../modules/utils.cpp -o tests -lSDL2 -lGL -lGLEW -lfreetype
tests: tests_main.cpp
clean: rm *.o
