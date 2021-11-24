#ifndef DICTIONARY_H
#define DICTIONARY_H

#define FONT_TEXTURE_ID 1

// Mouse buttons
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1

// Menu buttons IDs
#define MENU_BUTTON_NEWGAME 0
#define MENU_BUTTON_LOADGAME 1
#define MENU_BUTTON_SETTINGS 2
#define MENU_BUTTON_EXIT 3
#define MENU_BUTTON_NEWGAME_NAME 4
#define MENU_BUTTON_BACK 5
#define MENU_BUTTON_LOADGAME_NAME 6

// level ids have to match data/menu IDS 
#define MAIN_MENU_SCENE_ID 100
#define NEWGAME_MENU_SCENE_ID 101
#define LOADGAME_MENU_SCENE_ID 102
#define SETTINGS_MENU_SCENE_ID 103
#define IN_GAME_SCENE_ID 200

// camera types
#define CAMERA_DYNAMIC 0.0f
#define CAMERA_STATIC 1.0f
#define CAMERA_ZOOM_ONLY 2.0f

// object types
#define OBJECT_TYPE_ENTITY 0
#define OBJECT_TYPE_MAP 1
#define OBJECT_TYPE_MENU 2
#define OBJECT_TYPE_TEXT 3
#define OBJECT_TYPE_DEBUG 4
#define OBJECT_TYPE_GUI 5
#define OBJECT_TYPE_BUTTON 6

// entity collision sensors
#define SENSOR_TOP 0
#define SENSOR_TOP_RIGHT 1
#define SENSOR_RIGHT 2
#define SENSOR_BOTTOM_RIGHT 3
#define SENSOR_BOTTOM 4
#define SENSOR_BOTTOM_LEFT 5
#define SENSOR_LEFT 6
#define SENSOR_TOP_LEFT 7
#define SENSOR_CENTER 8

// entity collision AABB boxes
#define AABB_FULL 0

#define ENTITY_STATE_CALM 0
#define ENTITY_STATE_MOVING 1

#define NAVGATE_VERTICAL_ORIENTATION 0
#define NAVGATE_HORIZONTAL_ORIENTATION 1

// Log Levels
#define LOG_LVL_DEBUG 0
#define LOG_LVL_INFO 1
#define LOG_LVL_ERROR 2

// Log types
#define LOG_START_TIMER 0
#define LOG_END_TIMER 1 
#define LOG_INFO 2 
#define LOG_GAME_START 3
#define LOG_GAME_END 4


// GUI/MENU BUTTONS?
#define GUI_BUTTON_TRAVEL 0

#endif
