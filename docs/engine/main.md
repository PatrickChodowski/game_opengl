
# main

Path: main.cpp

Main module of the game, containing **int main()** method which has the main loop inside **while([game::RUNNING](game.md#RUNNING))**

Steps in the module:

1) Initialize SDL -> attributes, Window, OpenGL Context, Start Glew
2) [game::init()](game.md#init)
3) Start the while loop: Its going to run while game2::RUNNING is true

Inside the loop:

4) Check the start time
5) Read the input from the player: get the queued **SDL_Event event** and pass it to [events::handle_events()](events.md#handle_events);
6) [game::update()](game.md#update)
7) Swap Window with new data from opengl buffer
8) Time the end of the loop and add the delay to keep 60 frames per second

After loop is closed:

9) [game::drop()](game.md#drop)
10) Delete Opengl context and close the window, Quit SQL


## Tests:
No functions to test
