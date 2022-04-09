
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "game.h"
#include "saves.h"
#include "scenes.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace scenes
{
  phmap::flat_hash_map <int, sig_ptr> SceneLoader;
  void init()
  {
    scenes::SceneLoader[SCENE_ID_NEW_GAME_MENU] = _load_scene_new_game_menu;
    scenes::SceneLoader[SCENE_ID_LOAD_GAME_MENU] = _load_scene_load_game_menu;
    std::cout << "Scenes module initialized" << std::endl;
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