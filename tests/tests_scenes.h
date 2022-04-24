
#include "../modules/anims.h"
#include "../modules/buttons.h"
#include "../modules/ecs.h"
#include "../modules/events.h"
#include "../modules/mobs.h"
#include "../modules/mouse.h"
#include "../modules/npcs.h"
#include "../modules/shaders.h"
#include "../modules/game.h"
#include "../modules/scenes.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


bool test_scenes__game_init()
{
    bool passed = true;
    ecs::clear();
    game::clear();

    game::init();

    // Init not doing anything:
    // fonts, quads, logger, scripts

    // Not able to check:
    // buffer (if not initialized nothing will render anyway)

    // Checked on its own:
    // ecs

    // anims init
    if(anims::AnimsHandler.size() != 1){
      std::cout << " [ERROR] anims::Animshandler should be size 1 instead of " << anims::AnimsHandler.size() << std::endl;
      return false;
    }

    // buttons init
    if(buttons::ButtonFunctions.size() != 7){
      std::cout << " [ERROR] buttons::ButtonFunctions should be size 7 instead of " << buttons::ButtonFunctions.size() << std::endl;
      return false;
    }

    // camera init
    if(camera::CameraScale.size() != 3){
      std::cout << " [ERROR] camera::CameraScale should be size 3 instead of " << camera::CameraScale.size() << std::endl;
      return false;
    }

    // collisions init
    if(collisions::resolve.size() != 3){
      std::cout << " [ERROR] Collisions::resolve should be size 3 instead of " << collisions::resolve.size() << std::endl;
      return false;
    }

    // events init
    if(events::EventsHandler.size() != 4){
      std::cout << " [ERROR] events::EventsHandler should be size 4 instead of " << events::EventsHandler.size() << std::endl;
      return false;
    }

    // hero init
    if(hero::HeroLoader.size() != 3){
      std::cout << " [ERROR] hero::HeroLoader should be size 3 instead of " << hero::HeroLoader.size() << std::endl;
      return false;
    }

    // items init
    if(items::items.size() != 1){
      std::cout << " [ERROR] items::items should be size 1 instead of " << items::items.size() << std::endl;
      return false;
    }

    // maps init
    if(maps::maps.size() != 3){
      std::cout << " [ERROR] maps::maps should be size 3 instead of " << maps::maps.size() << std::endl;
      return false;
    }

    // mobs init
    if(mobs::mobs.size() != 2){
      std::cout << " [ERROR] mobs::mobs should be size 2 instead of " << mobs::mobs.size() << std::endl;
      return false;
    }

    // models init
    if(models::models.size() != 7){
      std::cout << " [ERROR] models::models should be size 7 instead of " << models::models.size() << std::endl;
      return false;
    }

    // mouse init
    if(mouse::ClickPriorities.size() != 4 ){
      std::cout << " [ERROR] mouse::ClickPriorities should be size 4 instead of " << mouse::ClickPriorities.size() << std::endl;
      return false;
    }

    if(mouse::click.size() != 4 ){
      std::cout << " [ERROR] mouse::click should be size 4 instead of " << mouse::click.size() << std::endl;
      return false;
    }

    // npcs init
    if(npcs::npcs.size() != 2){
      std::cout << " [ERROR] npcs::npcs should be size 2 instead of " << npcs::npcs.size() << std::endl;
      return false;
    }

    // scenes init
    if(scenes::SceneLoader.size() != 3){
      std::cout << " [ERROR] scenes::SceneLoader should be size 3 instead of " << scenes::SceneLoader.size() << std::endl;
      return false;
    }

    if(scenes::scenes.size() != 6){
      std::cout << " [ERROR] scenes::scenes should be size 6 instead of " << scenes::scenes.size() << std::endl;
      return false;
    }

    // shaders
    if(shaders::shaders.size() != 2){
      std::cout << " [ERROR] shaders::shaders should be size 2 instead of " << shaders::shaders.size() << std::endl;
      return false;
    }
    
    return passed;
}




bool test_scenes__game_clear()
{
  bool passed = false;




  if(true){
    passed = true;
  }
  return passed;
}