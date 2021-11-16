#include <fstream>
#include <string>

#include "camera.h"
#include "hero.h"
#include "saves.h"
#include "utils.h"

namespace saves2
{

  void _write_save_json(std::string& str_to_write)
  {
    std::string file_path = "./saves/"+hero2::hero.name+ ".json";
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
    sd.w = hero2::hero.w;
    sd.h = hero2::hero.h;
    sd.map_id = hero2::hero.map_id;
    sd.name = hero2::hero.name;
    sd.type = hero2::hero.type;
    sd.texture_id = hero2::hero.texture_id;
    sd.level = hero2::hero.level;
    sd.exp = hero2::hero.exp;
    sd.speed = hero2::hero.speed;
    sd.hp = hero2::hero.hp;
    sd.dmg = hero2::hero.dmg;
    sd.def = hero2::hero.def;
    sd.mobs_killed = hero2::hero.mobs_killed;

    std::string pretty_json = JS::serializeStruct(sd);
    saves2::_write_save_json(pretty_json);
  }


  void load_game(std::string name)
  { // loads hero, campaign, map and camera information

    SaveData SD;
    std::string data_path = "./saves/"+name+".json";
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);

    hero2::hero.name = SD.name;
    hero2::hero.type = SD.type;
    camera::cam.x = SD.x;
    camera::cam.y = SD.y;
    camera::cam.speed = SD.speed;
    hero2::hero.map_id = SD.map_id;
  }



}