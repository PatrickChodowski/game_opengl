#ifndef DICTIONARY_H
#define DICTIONARY_H

// level ids have to match data/menu IDS 
#define MAIN_MENU_LEVEL_ID 100
#define NEWGAME_MENU_LEVEL_ID 101
#define LOADGAME_MENU_LEVEL_ID 102
#define SETTINGS_MENU_LEVEL_ID 103
#define IN_GAME_LEVEL_ID 200

// camera types
#define CAMERA_DYNAMIC 0.0f
#define CAMERA_STATIC 1.0f
#define CAMERA_ZOOM_ONLY 2.0f

// object types
#define OBJECT_TYPE_ENTITY 0
#define OBJECT_TYPE_MAP 1
#define OBJECT_TYPE_MENU 2

// joints of the item to hero's body
#define JOINT_RIGHT_HAND 0
#define JOINT_LEFT_HAND 1
#define JOINT_UPPER_BODY 2
#define JOINT_LOWER_BODY 3
#define JOINT_LEGS 4
#define JOIN_HEAD 5 

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

// event ids:
#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define STAND_STILL 4
#define ATTACK 5

// frame names
#define FRAME_STAND_1 0
#define FRAME_STAND_2 1
#define FRAME_STAND_3 2
#define FRAME_LEFT_1 7
#define FRAME_LEFT_2 5
#define FRAME_LEFT_3 6
#define FRAME_RIGHT_1 9
#define FRAME_RIGHT_2 8
#define FRAME_RIGHT_3 10
#define FRAME_DOWN_1 4
#define FRAME_DOWN_2 3
#define FRAME_UP_1 12
#define FRAME_UP_2 13
#define FRAME_ATTACK_1 14
#define FRAME_ATTACK_2 15
#define FRAME_ATTACK_3 16

#define FRAME_DELAY_LONG 0.7
#define FRAME_DELAY 0.2
#define FRAME_DELAY_SHORT 0.08


#define ENTITY_STATE_CALM 0
#define ENTITY_STATE_MOVING 1

#define NAVGATE_VERTICAL_ORIENTATION 0
#define NAVGATE_HORIZONTAL_ORIENTATION 1


// logger codes:
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_ERROR 2 

#define LOG_EVENT_GAME_START 0
#define LOG_EVENT_GAME_EXIT 1 
#define LOG_EVENT_INIT_MODULE 2
#define LOG_EVENT_READ_DATA 3
#define LOG_EVENT_LOAD_QUADS 4
#define LOG_EVENT_DROP_DATA 5


// gui windows:
#define GUI_DEBUG 0

#endif
