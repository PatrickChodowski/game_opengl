#ifndef DICTIONARY_H
#define DICTIONARY_H

// Mouse buttons
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1

// camera types
#define CAMERA_DYNAMIC 0.0f
#define CAMERA_STATIC 1.0f
#define CAMERA_HIDDEN 2.0f

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


// Scenes
#define SCENE_ID_MAIN_MENU 0
#define SCENE_ID_NEW_GAME_MENU 1
#define SCENE_ID_LOAD_GAME_MENU 2
#define SCENE_ID_SETTINGS_MENU 3
#define SCENE_ID_DUNGEON_LEVEL_1 4
#define SCENE_ID_DUNGEON_LEVEL_2 5

// Scene Loading
#define SCENE_LOAD_FROM_NEW 0
#define SCENE_LOAD_FROM_LOAD 1
#define SCENE_LOAD_CHANGE_LEVEL 2

// Events handlers
#define EVENT_HANDLER_MENU 0
#define EVENT_HANDLER_NEW_GAME_MENU 1
#define EVENT_HANDLER_LOAD_GAME_MENU 2
#define EVENT_HANDLER_IN_GAME 3

// Travel states
#define TRAVEL_STATE_IDLE 0
#define TRAVEL_STATE_ACTIVE 1
#define TRAVEL_STATE_FINISHED 2

// Main animation types
#define ANIM_TYPE_FRAME 0
#define ANIM_TYPE_COLOR 1
#define ANIM_TYPE_POSITION 2
#define ANIM_TYPE_SIZE 3
#define ANIM_TYPE_FRAME_POSITION 4

// Animation Side Index
#define ANIM_SIDE_FRONT 1
#define ANIM_SIDE_BACK 2
#define ANIM_SIDE_RIGHT 3
#define ANIM_SIDE_LEFT 4
#define ANIM_SIDE_FRONT_RIGHT 5
#define ANIM_SIDE_FRONT_LEFT 6
#define ANIM_SIDE_BACK_RIGHT 7
#define ANIM_SIDE_BACK_LEFT 8 

// Animation index
#define ANIM_STANDING_IDLE 1
#define ANIM_UNARMED_WALK_FORWARD 2
#define ANIM_STANDING_TAUNT_CHEST_THUMP 3

// Entity types
#define ENTITY_TYPE_GUI 0
#define ENTITY_TYPE_LIVE 1
#define ENTITY_TYPE_BUTTON 2
#define ENTITY_TYPE_MAP 3
#define ENTITY_TYPE_TEXT 4
#define ENTITY_TYPE_DOOR 5
#define ENTITY_TYPE_COLOR 6
#define ENTITY_TYPE_ITEM 7 

// Component Index
#define COMPONENT_POSITION 0
#define COMPONENT_MODEL 1
#define COMPONENT_COLOR 2
#define COMPONENT_RENDERDATA 3
#define COMPONENT_BUTTON 4
#define COMPONENT_LABEL 5
#define COMPONENT_MOVE 6
#define COMPONENT_STATS 7
#define COMPONENT_COLLISIONS 8
#define COMPONENT_SENSORS 9
#define COMPONENT_ITEM 10
#define COMPONENT_EQUIPMENT 11
#define COMPONENT_NPC 12
#define COMPONENT_MOB 13

// Joint IDS (right hand, left hand etc.)
#define JOINT_RIGHT_HAND_ID 0
#define JOINT_LEFT_HAND_ID 1

// Item Location -> on ground/equipped
#define ITEM_LOCATION_GROUND 0
#define ITEM_LOCATION_EQ 1

#endif
