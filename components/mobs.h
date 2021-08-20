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



  void spawn(int map_id)
  { 
    // takes nests data from maps catalog using map id
    // spawns mobs on the level based on the level information

    // unpack each nest
    for(int i=0; i < maps::Catalog[map_id].nests.size(); i++)
    {
      quads::Quad mob_quad = ent::render_entity(ENTITY_TYPE_ID_MOB,
                                               true,
                                               6,
                                               0,
                                               maps::Catalog[map_id].nests[i].x,
                                               maps::Catalog[map_id].nests[i].y,
                                               128,
                                               128,
                                               0.0f,
                                               textures::FontTD,
                                               true,
                                               true
                                               );
      ent::EntityQuads.push_back(mob_quad);
    };
  }








}

#endif