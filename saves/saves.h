#ifndef SAVES_H
#define SAVES_H

namespace saves
{
  void write_save_json(std::string str_to_write)
  {
    std::string file_path = "./saves/"+CAMPAIGN_NAME+ ".json";
    std::ofstream json_file (file_path.c_str());
    if (json_file.is_open())
    {
      for(int i = 0; i < str_to_write.size(); i ++){
          json_file << str_to_write[i];
      }
      json_file.close();
    }
  }

  struct SaveData
  {
    int x;
    int y;
    int map_id;
    std::string name;
    std::string type;
    int texture_id;
    int level;
    int exp;
    int speed;
    int hp;
    int dmg;
    int def;
    int mobs_killed;

    JS_OBJ(x, y, map_id, name, type, texture_id, 
    level, exp, speed, hp,dmg,def,mobs_killed);
  };

  void save_game()
  {
    // uses current campaign name to save to file
    SaveData sd;
    sd.x = camera::x;
    sd.y = camera::y;
    sd.map_id = MAP_ID;
    sd.name = CAMPAIGN_NAME;
    sd.type = "barbarian";
    sd.texture_id = 3;
    sd.level = 1;
    sd.exp = 0;
    sd.speed = camera::speed;
    sd.hp = 100;
    sd.dmg = 20;
    sd.def = 100;
    sd.mobs_killed = 0;

    std::string pretty_json = JS::serializeStruct(sd);
    // std::cout << pretty_json << std::endl;
    saves::write_save_json(pretty_json);
  }

  void load_game(std::string name)
  { // loads hero, campaign, map and camera information

    SaveData SD;
    std::string data_path = "./saves/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);
  }





}















#endif