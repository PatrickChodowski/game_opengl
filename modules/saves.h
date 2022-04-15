
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

  // Just for recognizing load game name
  extern phmap::flat_hash_map<int, std::string> saves_buttons_map;

  struct SaveData
  {
    std::string name;
    std::vector<unsigned int> components;
    
    int model_id;
    int scene_id;
  
    // Position
    float x, y;

    // Dimension
    float w, h;

    // Stats
    int level, mobs_killed;
    float hp, dmg, def, speed, exp;

    // Collisions
    float radius;

    //std::vector<npcs::InteractionData> interactions;
    JS_OBJ(name, components, model_id, scene_id, x, y, w, h, 
           level, mobs_killed, hp, dmg, def, speed, exp, radius);
  };

  // Writes the save json file to saves directory
  void _write_save_json(std::string& str_to_write);

  // Saves game data to struct and saves it further to the file
  void save_game();

  // Load game data from the file
  void load_game(std::string& name);

  // Reads save data fom file
  saves::SaveData read_data(std::string& name);

  // Update list of saves
  void list_saves();

  // Validate new game name
  bool _validate_name();

  // Validate input to new game name
  bool _validate_input(std::string input);


  

}


#endif