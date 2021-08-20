#ifndef DICTIONARY_H
#define DICTIONARY_H

// file containing all definitions


#define QUAD_TYPE_MAP 0.0f
#define QUAD_TYPE_MENU 1.0f
#define QUAD_TYPE_TEXT 2.0f
#define QUAD_TYPE_ENTITY 3.0f

#define ENTITY_TYPE_ID_HERO 0
#define ENTITY_TYPE_ID_ITEM 1
#define ENTITY_TYPE_ID_MOB 2
#define ENTITY_TYPE_ID_NA 9

#define ENTITY_NOT_STATIC 0.0f
#define ENTITY_STATIC 1.0f

// joints of the item to hero's body
#define JOINT_RIGHT_HAND 0
#define JOINT_LEFT_HAND 1
#define JOINT_UPPER_BODY 2
#define JOINT_LOWER_BODY 3
#define JOINT_LEGS 4
#define JOIN_HEAD 5 

// ENTITY SENSORS
#define SENSOR_TOP 0
#define SENSOR_TOP_RIGHT 1
#define SENSOR_RIGHT 2
#define SENSOR_BOTTOM_RIGHT 3
#define SENSOR_BOTTOM 4
#define SENSOR_BOTTOM_LEFT 5
#define SENSOR_LEFT 6
#define SENSOR_TOP_LEFT 7
#define SENSOR_CENTER 8

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

#endif
