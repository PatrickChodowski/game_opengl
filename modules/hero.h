#include <string>
#include "utils.h"
#include "../dependencies/json_struct.h"

#ifndef MODULES_HERO_H
#define MODULES_HERO_H

// Persistent data. Will contain:
// campaign information: quests, statuses 
// current status like position 
// statistics 
// etc.
// should not have much about the quads, entities etc, its mostly data here
// any logic here? dont think so

namespace hero
{

  struct HeroData
  {
    // position
    float x, y;
    float w, h;

    // Previous position for collisions
    float prev_x, prev_y;

    int entity_id;
    int current_frame;
    int map_id;

    // display information
    int current_movement_state;
    int texture_id;
    int level;
    int mobs_killed;

    // statistics
    float exp;
    float speed;
    float hp;
    float dmg;
    float def;

    // personal information
    std::string name;
    std::string type;

    JS_OBJ(x, y, w, h, map_id, texture_id, level, exp, speed, hp, dmg, def, mobs_killed, name, type);
  }; 

  extern hero::HeroData hero;

  // Reads default data for character type 
  void _read_data(std::string char_type);

  // Loads hero struct information from save 
  void _load_from_save(std::string save_name);

  // Revert to previous hero position x 
  void revert_position_x();

  // Revert to previous hero position y
  void revert_position_y();
  
  // Update hero position
  void update_position(float camera_move_x, float camera_move_y);

  // Creates new character of certain type. Will have default stats
  void create_new(std::string name, std::string type);

}

#endif
