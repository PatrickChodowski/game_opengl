
#include "../modules/game.h"

bool test_game__check_if_menu_scene_1()
{
  bool passed = false;
  game::SCENE_ID = 1;
  game::_check_if_menu();
  if(game::IS_MENU == 200){passed = true;}
  return passed;
}

bool test_game__check_if_menu_scene_100()
{
  bool passed = false;
  game::SCENE_ID = 100;
  game::_check_if_menu();
  if(game::IS_MENU == 100){passed = true;}
  return passed;
}

bool test_game__check_if_menu_scene_150()
{
  bool passed = false;
  game::SCENE_ID = 150;
  game::_check_if_menu();
  if(game::IS_MENU == 150){passed = true;}
  return passed;
}

