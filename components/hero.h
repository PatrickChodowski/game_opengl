#ifndef HERO_H
#define HERO_H

namespace hero
{
  struct Hero
  {
    // position
    int x;
    int y;

    // display information
    int current_movement_state;
    
    // personal information
    std::string name;
    int level;

    // statistics
    int exp;
    int speed;
    int hp;
    int dmg;
    int def;
    int mobs_killed;

  }; 


}



#endif