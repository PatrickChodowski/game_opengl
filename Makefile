output: game
	g++ -std=c++17 modules/py.cpp -o game -lSDL2 -lGL -lGLEW -lfreetype -lpython3.8 -lboost_python38 -I /usr/include/python3.8/
game: main.cpp
clean: rm *.o

