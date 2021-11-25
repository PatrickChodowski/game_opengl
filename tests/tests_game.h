
#include "../modules/game.h"

bool test_game__check_if_menu_scene_1()
{
  bool passed = false;
  game2::SCENE_ID = 1;
  game2::_check_if_menu();
  if(game2::IS_MENU == 200){passed = true;}
  return passed;
}

bool test_game__check_if_menu_scene_100()
{
  bool passed = false;
  game2::SCENE_ID = 100;
  game2::_check_if_menu();
  if(game2::IS_MENU == 100){passed = true;}
  return passed;
}

bool test_game__check_if_menu_scene_150()
{
  bool passed = false;
  game2::SCENE_ID = 150;
  game2::_check_if_menu();
  if(game2::IS_MENU == 150){passed = true;}
  return passed;
}

