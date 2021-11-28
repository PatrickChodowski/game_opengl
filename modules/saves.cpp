#include <fstream>
#include <string>

#include "camera.h"
#include "hero.h"
#include "saves.h"
#include "utils.h"

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
    sd.x = camera::cam.x;
    sd.y = camera::cam.y;
    sd.w = hero::hero.w;
    sd.h = hero::hero.h;
    sd.map_id = hero::hero.map_id;
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


  void load_game(std::string name)
  { // loads hero, campaign, map and camera information

    SaveData SD;
    std::string data_path = "./saves/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);

    hero::hero.name = SD.name;
    hero::hero.type = SD.type;
    camera::cam.x = SD.x;
    camera::cam.y = SD.y;
    hero::hero.speed = SD.speed;
    hero::hero.map_id = SD.map_id;
  }



}