#ifndef DICTIONARY_H
#define DICTIONARY_H

#define FONT_TEXTURE_ID 1

// Mouse buttons
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1

// camera types
#define CAMERA_DYNAMIC 0.0f
#define CAMERA_STATIC 1.0f

// entity types
#define ENTITY_TYPE_MOB 0
#define ENTITY_TYPE_ITEM 1
#define ENTITY_TYPE_HERO 2
#define ENTITY_TYPE_NPC 3

// object types
#define OBJECT_TYPE_ENTITY 0
#define OBJECT_TYPE_MAP 1
#define OBJECT_TYPE_MENU 2
#define OBJECT_TYPE_TEXT 3
#define OBJECT_TYPE_DEBUG 4
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

// Buttons
#define BUTTON_NEWGAME 0
#define BUTTON_LOADGAME 1
#define BUTTON_SETTINGS 2
#define BUTTON_EXIT 3
#define BUTTON_NEWGAME_NAME 4
#define BUTTON_BACK 5
#define BUTTON_LOADGAME_NAME 6
#define BUTTON_TRAVEL 7

// Scenes
#define SCENE_ID_MAIN_MENU 0
#define SCENE_ID_NEW_GAME_MENU 1
#define SCENE_ID_LOAD_GAME_MENU 2
#define SCENE_ID_SETTINGS_MENU 3
#define SCENE_ID_DUNGEON_LEVEL_1 4

// Events handlers
#define EVENT_HANDLER_MENU 0
#define EVENT_HANDLER_NEW_GAME_MENU 1
#define EVENT_HANDLER_LOAD_GAME_MENU 2
#define EVENT_HANDLER_IN_GAME 3

// Menu Slots
#define MENU_SLOT_FULL_SCREEN 0
#define MENU_SLOT_RIGHT_SIDE_1 1
#define MENU_SLOT_RIGHT_SIDE_2 2

// Menu types
#define MENU_MAIN_ID 1
#define MENU_NEWGAME_ID 2
#define MENU_LOADGAME_ID 3
#define MENU_SETTINGS_ID 4
#define MENU_ENTITY_ID 5

// Travel states
#define TRAVEL_STATE_IDLE 0
#define TRAVEL_STATE_ACTIVE 1
#define TRAVEL_STATE_FINISHED 2

#endif
