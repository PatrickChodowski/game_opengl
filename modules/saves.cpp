#include <iostream>
#include <fstream>
#include <string>

#include "camera.h"
#include "game.h"
#include "hero.h"
#include "npcs.h"
#include "saves.h"
#include "scenes.h"
#include "utils.h"

#include "../dictionary.h"

namespace saves
{
  std::vector<std::string> saves;
  std::string NewGameName;
  std::string LoadGameName;
  phmap::flat_hash_map<int, std::string> saves_buttons_map;

  // Updated from scenes::_load_scene_new_game_menu. Used only for the new game name 
  int NEW_GAME_NAME_BUTTON_ENTITY;
  const std::string _allowed_input = "abcdefghijklmnoprstuvwxyz";

  void _write_save_json(std::string& str_to_write)
  {
    std::string file_path = "./saves/"+ecs::entities.at(hero::HERO_ENTITY_ID).name+ ".json";
    std::ofstream json_file (file_path);
    if (json_file.is_open())
    {
      for(int i = 0; i < str_to_write.size(); i ++){
          json_file << str_to_write[i];
      }
      json_file.close();
    }
  }

  void save_game()
  {
    // uses current campaign name to save to file
    // SaveData sd;
    // sd.x = hero::hero.x;
    // sd.y = hero::hero.y;
    // sd.w = hero::hero.w;
    // sd.h = hero::hero.h;
    // sd.scene_id = game::SCENE_ID;
    // sd.name = hero::hero.name;
    // sd.type = hero::hero.type;
    // sd.model_id = hero::hero.model_id;
    // sd.level = hero::hero.level;
    // sd.exp = hero::hero.exp;
    // sd.speed = hero::hero.speed;
    // sd.hp = hero::hero.hp;
    // sd.dmg = hero::hero.dmg;
    // sd.def = hero::hero.def;
    // sd.mobs_killed = hero::hero.mobs_killed;

    // sd.interactions = npcs::interactions;

    // std::string pretty_json = JS::serializeStruct(sd);
    // saves::_write_save_json(pretty_json);
  }

  saves::SaveData read_save_data(std::string& name)
  {    
    SaveData SD;
    std::string data_path = "./saves/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);
    return SD;
  }

  void load_game(std::string& name)
  { 
    // loads hero, campaign, map and camera information
    // SaveData SD = saves::read_save_data(name);
    // hero::hero.h = SD.h;
    // hero::hero.w = SD.w;
    // hero::hero.model_id = SD.model_id;
    // hero::hero.name = SD.name;
    // hero::hero.type = SD.type;
    // hero::hero.speed = SD.speed;
    // hero::hero.current_frame = 0;
    // save rest of the statistics here later

    // hero::hero.entity_id = entity::create(hero::hero, ENTITY_TYPE_HERO, CAMERA_DYNAMIC);
    // hero::set_position(SD.x, SD.y);
    // camera::cam.x = (SD.x - (game::WINDOW_WIDTH/2) + (hero::hero.w/2));
    // camera::cam.y = - (SD.y - (game::WINDOW_HEIGHT/2) + (hero::hero.h/2));

    // game::SCENE_ID = SD.scene_id;
    // game::MAP_ID = scenes::scenes[game::SCENE_ID].map_id;
    // npcs::interactions = SD.interactions;
  }

  void list_saves()
  {
    saves::saves_buttons_map.clear();
    saves::saves = utils::list_json_files("saves");
  };

  bool _validate_input(std::string input)
  {
    bool good = false;
    if (saves::_allowed_input.find(input) != std::string::npos){
      good = true;
    }
    return good;
  };

  bool _validate_name()
  {
    bool good = false;
    // check if doesnt exist in saves
    // check if has more than 0 characters

    if(saves::NewGameName.size() > 0){
      for(int s = 0; s<saves::saves.size(); s++)
      {
        if(saves::NewGameName == saves[s]){
          return good;
        }
      }
      good = true;
    }
    return good;
  };



}