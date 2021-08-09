#ifndef HERO_H
#define HERO_H

/// how to render hero and also how to update only the correct part of the buffer?
// will be a bluebrint for other renderable entities
namespace hero
{

  float HERO_X = ((float)WINDOW_WIDTH)/2 - (70/2);
  float HERO_Y = ((float)WINDOW_HEIGHT)/2 - (90/2) ;


  struct Hero
  {
    // position
    int x;
    int y;
    int map_id;

    // display information
    int current_movement_state;
    
    // personal information
    std::string name;
    std::string type;
    int texture_id;
    int level;

    // statistics
    int exp;
    int speed;
    int hp;
    int dmg;
    int def;
    int mobs_killed;

    JS_OBJ(x, y, map_id, name, type, texture_id, level, exp, speed, hp, dmg, def, mobs_killed);
  }; 

  Hero load_from_save(std::string save_name)
  {

    Hero hero;
    std::string file_path = "saves/" + save_name + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero);

    if(LOGGING == 0)
    {
      std::cout << "Loaded Hero from save: " << save_name << std::endl
      << " name: " << hero.name << " type: " << hero.type << std::endl
      << " position: " << hero.x << "," << hero.y << " map_id: " << hero.map_id << std::endl
      << " texture_id: " << hero.texture_id << " experience: " << hero.exp << std::endl
      << " hp: " << hero.hp << " speed: " << hero.speed << " dmg: " << hero.dmg << std::endl
      << " def: " << hero.def << " mobs killed: " << hero.mobs_killed << std::endl;
    }

    return hero;
  }



}



#endif