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

namespace hero2
{

  struct HeroData
  {
    // position
    int x, y;
    int w, h;
    int current_frame;
    int map_id;

    // display information
    int current_movement_state;
    int texture_id;
    int level;

    // statistics
    int exp;
    int speed;
    int hp;
    int dmg;
    int def;
    int mobs_killed;

    // personal information
    std::string name;
    std::string type;

    JS_OBJ(x, y, map_id, texture_id, level, exp, speed, hp, dmg, def, mobs_killed, name, type);
  }; 

  extern hero2::HeroData hero;

  // Loads hero struct information from save 
  void _load_from_save(std::string save_name);

}

#endif