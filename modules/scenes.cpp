
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "fonts.h"
#include "game.h"
#include "maps.h"
#include "saves.h"
#include "scenes.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace scenes
{
  phmap::flat_hash_map<int, sig_ptr> SceneLoader;
  phmap::flat_hash_map<int, scenes::SceneData> scenes;

  void init()
  {
    scenes::SceneLoader[SCENE_ID_NEW_GAME_MENU] = _load_scene_new_game_menu;
    scenes::SceneLoader[SCENE_ID_LOAD_GAME_MENU] = _load_scene_load_game_menu;

    std::vector<std::string> scene_list = utils::list_json_files("data/scenes");
    for(int s=0; s < scene_list.size(); s++)
    {
      scenes::read_data(scene_list[s]);
    };

    std::cout << "Scenes initialized" << std::endl;
  }

  void read_data(std::string& name)
  {
    scenes::SceneData SD;
    std::string data_path = "./data/scenes/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);
    scenes::scenes.insert({SD.id, SD});
  };

  void load(int scene_id, int load_scene_from)
  {
    std::cout << "TU? " << scene_id  << " load scene from " << load_scene_from << std::endl;
    if(scenes::scenes.count(scene_id) > 0)
    {    
      std::cout << "TU2?" << std::endl;
      models::load(fonts::FONT_MODEL_ID);
      game::SCENE_ID = scene_id;
      game::EVENT_HANDLER_ID = scenes::scenes[scene_id].events_handler_id;
      game::MAP_ID = scenes::scenes[scene_id].map_id;
      game::HeroLoader[load_scene_from](scene_id);

      // Load map
      maps::init_map(game::MAP_ID);

      // Load entities
      for(int e=0; e<scenes::scenes[scene_id].entities.size(); e++)
      {
        ecs::create_entity_from_file(scenes::scenes[scene_id].entities[e]);
      }

      // Adding dynamic logic based on the scene ID
      if(scenes::SceneLoader.count(scene_id) > 0){
        scenes::SceneLoader.at(scene_id)();
      }

      // // load mobs based on the map
      // mobs::spawn_from_nest(game::MAP_ID);

      // // Spawns npcs for the map
      // npcs::spawn_from_map(game::MAP_ID);

      //items::spawn(1, 200, 300);

      std::cout << " [SCENE] Loaded Scene: " << scene_id << std::endl;
    }
  }

  void switch_scene(int scene_id, int load_scene_from)
  {
    game::clear();
    scenes::load(scene_id, load_scene_from);
    game::SCENE_ID = scene_id;

    // have to be cleared after loading the scene
    saves::NewGameName = "";
    saves::LoadGameName = "";
    saves::NEW_GAME_NAME_BUTTON_ENTITY = -1;

    std::cout << " [SCENE] Finished switching scenes " << std::endl;
  }

  void _load_scene_new_game_menu()
  {
    std::cout << " [SCENES] Dynamic scene logic for scene " << SCENE_ID_NEW_GAME_MENU << std::endl;
    ecs::TempEntityData e;
    e.name = "new_game_name";
    e.components = {0,1,2,3,4,5,6};
    e.entity_type_id = ENTITY_TYPE_BUTTON;
    e.x = 340;
    e.y = 400;
    e.z = 0.3;
    e.w = 240;
    e.h = 70;
    e.r = 0.701;
    e.g = 0.675;
    e.b = 0.675;
    e.a = 1.0;
    e.model_id = -1;
    e.frame_id = -1;
    e.side_id = 0;
    e.camera_type = CAMERA_STATIC;
    e.button_function_id = BUTTON_NEWGAME_NAME;
    e.label = "";
    e.text_size = 45;
    e.text_r = 0.5;
    e.text_g = 0.5;
    e.text_b = 0.5;
    e.text_a = 1.0;
    e.text_x = 350;
    e.text_y = 410;
    e.text_z = 1.0;

    // Remember button entity id 
    saves::NEW_GAME_NAME_BUTTON_ENTITY = ecs::create_entity(&e);
  }

  void _load_scene_load_game_menu()
  {
    std::cout << " [SCENES] Dynamic scene logic for scene " << SCENE_ID_LOAD_GAME_MENU << std::endl;
    saves::list_saves();
    float start_x = 340;
    float start_y = 400;

    for(int s=0;s<saves::saves.size();s++)
    {
      ecs::TempEntityData e;
      e.name = "load_game_" + saves::saves[s];
      e.components = {0,1,2,3,4,5,6};
      e.entity_type_id = ENTITY_TYPE_BUTTON;
      e.x = start_x;
      e.y = start_y;
      e.z = 0.3;
      e.w = 240;
      e.h = 70;
      e.r = 0.701;
      e.g = 0.675;
      e.b = 0.675;
      e.a = 1.0;
      e.model_id = -1;
      e.frame_id = -1;
      e.side_id = 0;
      e.camera_type = CAMERA_STATIC;
      e.button_function_id = BUTTON_LOADGAME_NAME;
      e.label = saves::saves[s];
      e.text_size = 45;
      e.text_r = 0.5;
      e.text_g = 0.5;
      e.text_b = 0.5;
      e.text_a = 1.0;
      e.text_x = start_x + 10;
      e.text_y = start_y + 10;
      e.text_z = 1.0;
      ecs::create_entity(&e);
      start_y += 80;
    }
  }


}