

#include <string>
#include <vector>

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"

#ifndef MODULES_HERO_H
#define MODULES_HERO_H

namespace hero 
{
  // Entity of hero character
  extern int HERO_ENTITY_ID;

  typedef void (*sig_ptr)(int scene_id);
  // Functions map to loading a hero
  extern phmap::flat_hash_map<int, sig_ptr> HeroLoader;

  struct HeroData
  {
    int model_id;
    float hp;
    float dmg;
    float def;
    float speed;
    float w;
    float h;
    float radius_x;
    float radius_y;

    JS_OBJ(model_id, hp, dmg, def, speed, w, h, radius_x, radius_y);
  };
  
  // Initialize hero character
  void init();

  // Read data based on character type
  hero::HeroData read_data(std::string char_type);

  // Loads hero from new game logic
  void _load_hero_from_new_game(int scene_id);

  // Loads hero from load game logic
  void _load_hero_from_load_game(int scene_id);

  // Loads hero from change level logic
  void _load_hero_from_change_level(int scene_id);

  // Create new hero with name, type, X,Y
  void create_new(std::string name, std::string type, float x, float y);
}


#endif
