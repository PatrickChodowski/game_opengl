
#include <iostream>
#include <string>
#include <vector>

#include "camera.h"
#include "ecs.h"
#include "hero.h"
#include "game.h"
#include "models.h"
#include "saves.h"
#include "scenes.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"

namespace hero
{
  int HERO_ENTITY_ID;
  phmap::flat_hash_map<int, sig_ptr> HeroLoader;

  void init()
  {
    hero::HeroLoader[SCENE_LOAD_FROM_NEW] = hero::_load_hero_from_new_game;
    hero::HeroLoader[SCENE_LOAD_FROM_LOAD] = hero::_load_hero_from_load_game;
    hero::HeroLoader[SCENE_LOAD_CHANGE_LEVEL] = hero::_load_hero_from_change_level;

    std::cout << "Hero initialized" << std::endl;
  };

  hero::HeroData read_data(std::string char_type)
  {
    hero::HeroData HD;
    std::string file_path = "data/heros/" + char_type + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(HD);
    return HD;
  }

  void _load_hero_from_new_game(int scene_id)
  {
    std::cout << " [GAME] Loading hero from new game to scene ID: " << scene_id << std::endl;
    float hero_start_x = scenes::scenes[scene_id].hero_start_x;
    float hero_start_y = scenes::scenes[scene_id].hero_start_y;

    // if(!scenes::scenes[scene_id].is_gp){
    //   hero::refresh();
    // }
    
    if(hero_start_x != -1000 & hero_start_y != -1000){
      hero::create_new(saves::NewGameName, "barbarian", hero_start_x, hero_start_y);
      camera::cam.x = (hero_start_x - (game::WINDOW_WIDTH/2) + (ecs::dimensions.at(hero::HERO_ENTITY_ID).w/2));
      camera::cam.y = - (hero_start_y - (game::WINDOW_HEIGHT/2) + (ecs::dimensions.at(hero::HERO_ENTITY_ID).h/2));
    }
  };

  void _load_hero_from_load_game(int scene_id)
  {
    // if(!scenes::scenes[scene_id].is_gp){
    //   hero::refresh();
    // }
    std::cout << " [HERO] Load hero from game name: " << saves::LoadGameName << std::endl;
    saves::load_game(saves::LoadGameName);
  };

  void _load_hero_from_change_level(int scene_id)
  {
    // if(game::HERO_START_X != -1000 & game::HERO_START_Y != -1000)
    // {
    //   //hero::hero.entity_id = entity::create(hero::hero, ENTITY_TYPE_HERO, CAMERA_DYNAMIC);
    //   hero::set_position(game::HERO_START_X, game::HERO_START_Y);
    //   camera::cam.x = (game::HERO_START_X - (game::WINDOW_WIDTH/2) + (hero::hero.w/2));
    //   camera::cam.y = - (game::HERO_START_Y - (game::WINDOW_HEIGHT/2) + (hero::hero.h/2));
    // }
  };

  void create_new(std::string name, std::string type, float x, float y)
  {
    hero::HeroData HD = hero::read_data(type);
    ecs::TempEntityData e;
    e.name = name;
    e.components = {0,1,2,3,4,7,8};
    e.entity_type_id = ENTITY_TYPE_LIVE;
    e.x = x;
    e.y = y;
    e.z = 0.3;
    e.w = 200;
    e.h = 200;
    e.r = 0.5;
    e.g = 0.5;
    e.b = 0.5;
    e.a = 1.0;
    e.model_id = HD.model_id;
    e.frame_id = 10100;
    e.side_id = ANIM_SIDE_FRONT;
    e.camera_type = CAMERA_DYNAMIC;

    e.level = 1;
    e.exp = 0;
    e.speed = HD.speed;
    e.hp = HD.hp;
    e.dmg = HD.dmg;
    e.def = HD.def;

    hero::HERO_ENTITY_ID = ecs::create_entity(&e);
    std::cout << " [HERO] Created new character entity ID: " << hero::HERO_ENTITY_ID << std::endl;
  };
}

