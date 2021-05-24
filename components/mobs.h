#ifndef MOBS_H
#define MOBS_H


namespace mobs
{
  
  struct Mob
  {
    int entity_id;
    std::string type;
    int texture_id;

    int x;
    int y;
    int w;
    int h;

    int hp;
    int speed;
    int dmg;
    int def; 
  };

  // entity ids of alive mobs
  std::vector<int> alive_mobs = {};
  std::vector<int> level_mobs = {};
  std::map<int, Mob> Catalog = {};
}

#endif