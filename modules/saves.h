
#include <map>
#include <string>
#include <vector>

#ifndef MODULES_SAVES_H
#define MODULES_SAVES_H


namespace saves
{

  struct SaveData
  {
    float x;
    float y;
    float w;
    float h;

    float exp;
    float speed;
    float hp;
    float dmg;
    float def;

    int map_id;
    int texture_id;
    int level;
    int mobs_killed;

    std::string name;
    std::string type;

    JS_OBJ(x, y, w, h, exp, speed, hp, dmg, def, map_id, texture_id, 
    level, mobs_killed, name, type);
  };

  // Writes the save json file to saves directory
  void _write_save_json(std::string& str_to_write);

  // Saves game data to struct and saves it further to the file
  void save_game();
  
  // Load game data from the file
  void load_game();
  

}


#endif