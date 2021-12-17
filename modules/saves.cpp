#include <iostream>
#include <fstream>
#include <string>

#include "camera.h"
#include "entity.h"
#include "game.h"
#include "hero.h"
#include "saves.h"
#include "utils.h"

#include "../dictionary.h"

namespace saves
{

  void _write_save_json(std::string& str_to_write)
  {
    std::string file_path = "./saves/"+hero::hero.name+ ".json";
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
    SaveData sd;
    sd.x = hero::hero.x;
    sd.y = hero::hero.y;
    sd.w = hero::hero.w;
    sd.h = hero::hero.h;
    sd.scene_id = game::SCENE_ID;
    sd.map_id = game::scenes[game::SCENE_ID].map_id;
    sd.name = hero::hero.name;
    sd.type = hero::hero.type;
    sd.texture_id = hero::hero.texture_id;
    sd.level = hero::hero.level;
    sd.exp = hero::hero.exp;
    sd.speed = hero::hero.speed;
    sd.hp = hero::hero.hp;
    sd.dmg = hero::hero.dmg;
    sd.def = hero::hero.def;
    sd.mobs_killed = hero::hero.mobs_killed;

    std::string pretty_json = JS::serializeStruct(sd);
    saves::_write_save_json(pretty_json);
  }


  void load_game(std::string& name)
  { // loads hero, campaign, map and camera information

    SaveData SD;
    std::string data_path = "./saves/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);

    hero::hero.h = SD.h;
    hero::hero.w = SD.w;
    hero::hero.texture_id = SD.texture_id;
    hero::hero.name = SD.name;
    hero::hero.type = SD.type;
    hero::hero.speed = SD.speed;
    hero::hero.current_frame = 0;
    // save rest of the statistics here later

    hero::hero.entity_id = entity::create(hero::hero, ENTITY_TYPE_HERO, CAMERA_DYNAMIC);
    hero::set_position(SD.x, SD.y);
    camera::cam.x = (SD.x - (game::WINDOW_WIDTH/2) + (hero::hero.w/2));
    camera::cam.y = - (SD.y - (game::WINDOW_HEIGHT/2) + (hero::hero.h/2));

    game::SCENE_ID = SD.scene_id;
    game::MAP_ID = SD.map_id;
  }



}