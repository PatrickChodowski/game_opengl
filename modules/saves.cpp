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
    //uses current campaign name to save to file
    saves::SaveData sd;

    ecs::EntityData e = ecs::entities.at(hero::HERO_ENTITY_ID);
    ecs::PositionComponent pos = ecs::positions.at(hero::HERO_ENTITY_ID);
    ecs::StatsComponent stats = ecs::stats.at(hero::HERO_ENTITY_ID);

    sd.name = e.name;
    sd.components = e.components;
    sd.model_id = ecs::models.at(hero::HERO_ENTITY_ID).model_id;
    sd.scene_id = game::SCENE_ID;
    sd.x = pos.x;
    sd.y = pos.y;
    sd.w = pos.w;
    sd.h = pos.h;

    sd.level = stats.level;
    sd.mobs_killed = stats.mobs_killed;
    sd.hp = stats.hp;
    sd.dmg = stats.dmg;
    sd.def = stats.def;
    sd.speed = stats.speed;
    sd.exp = stats.exp;

    std::string pretty_json = JS::serializeStruct(sd);
    saves::_write_save_json(pretty_json);
  }

  saves::SaveData read_data(std::string& name)
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
    SaveData SD = saves::read_data(name);
    ecs::TempEntityData e;
    e.name = SD.name;
    e.components = SD.components;
    e.entity_type_id = ENTITY_TYPE_LIVE;
    e.x = SD.x;
    e.y = SD.y;
    e.z = 0.3;
    e.w = SD.w;
    e.h = SD.h;
    e.r = 0.5;
    e.g = 0.5;
    e.b = 0.5;
    e.a = 1.0;
    e.model_id = SD.model_id;
    e.frame_id = 10100;
    e.side_id = ANIM_SIDE_FRONT;
    e.camera_type = CAMERA_DYNAMIC;

    e.level = SD.level;
    e.mobs_killed = SD.mobs_killed;
    e.exp = SD.exp;
    e.speed = SD.speed;
    e.hp = SD.hp;
    e.dmg = SD.dmg;
    e.def = SD.def;

    e.radius_x = SD.radius_x;
    e.radius_y = SD.radius_y;
    e.animated = true;

    camera::cam.x = (SD.x - (game::WINDOW_WIDTH/2) + (SD.w/2));
    camera::cam.y = - (SD.y - (game::WINDOW_HEIGHT/2) + (SD.h/2));

    hero::HERO_ENTITY_ID = ecs::create_entity(&e);
    std::cout << " [SAVES] Loaded game from save: " << name << " Hero entity ID: " << hero::HERO_ENTITY_ID << std::endl;
  };

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