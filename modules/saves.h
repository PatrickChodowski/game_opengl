
#include <string>
#include <vector>

#include "npcs.h"

#ifndef MODULES_SAVES_H
#define MODULES_SAVES_H


namespace saves
{
  // List of save files
  extern std::vector<std::string> saves;
  extern std::string NewGameName;
  extern std::string LoadGameName;
  extern int NEW_GAME_NAME_BUTTON_ENTITY;

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

    int scene_id;
    int model_id;
    int level;
    int mobs_killed;

    std::string name;
    std::string type;


    std::vector<npcs::InteractionData> interactions;


    JS_OBJ(x, y, w, h, exp, speed, hp, dmg, def, scene_id, model_id, 
    level, mobs_killed, name, type, interactions);
  };

  // Writes the save json file to saves directory
  void _write_save_json(std::string& str_to_write);

  // Saves game data to struct and saves it further to the file
  void save_game();

  // Load game data from the file
  void load_game(std::string& name);

  // Reads save data fom file
  saves::SaveData read_save_data(std::string& name);

  // Update list of saves
  void list_saves();

  // Validate new game name
  bool _validate_name();

  // Validate input to new game name
  bool _validate_input(std::string input);


  

}


#endif