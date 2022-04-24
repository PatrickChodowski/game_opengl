
#include "../modules/anims.h"
#include "../modules/buttons.h"
#include "../modules/ecs.h"
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
    // quads

    // Not able to check:
    // buffer (if not initialized nothing will render anyway)

    // anims init
    if(anims::AnimsHandler.size() != 1){

        return false;
    }

    // buttons init
    if(buttons::ButtonFunctions.size() != 7){

        return false;
    }

    // camera init
    if(camera::CameraScale.size() != 3){

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