

#include "ecs.h"
#include "maps.h"
#include "mobs.h"
#include "models.h"
#include "quads.h"
#include "utils.h"

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#include <iostream>
#include <string>
#include <vector>

namespace mobs
{
  phmap::flat_hash_map<int, MobData> mobs = {};

  void read_data(std::string name)
  {
    mobs::MobData MD;
    std::string data_path = "./data/mobs/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    mobs::mobs.insert({MD.mob_id, MD});
  };

  void init()
  {
    std::vector<std::string> mob_list = utils::list_json_files("./data/mobs/");
    for(int m=0; m < mob_list.size(); m++)
    {
      mobs::read_data(mob_list[m]);
    }
    std::cout << "Mobs Initialized" << std::endl;
  };

  void refresh()
  {
    mobs::mobs.clear();
  }

  void spawn_from_nest(int map_id)
  {
    if(map_id > -1)
    {
      int MOB_ID = 0;
      // unpack each nest
      for(int i=0; i < maps::maps[map_id].nests.size(); i++)
      {
        // for each mob of the nest
        for(int m=0; m<maps::maps[map_id].nests[i].n; m++)
        {
          mobs::spawn(MOB_ID, maps::maps[map_id].nests[i].x, maps::maps[map_id].nests[i].y);
        }
      }
    }
  };

  int spawn(int mob_id, float x, float y)
  {
    std::cout << " [MOBS] Spawning mob from MOB ID " << mob_id << std::endl;
    mobs::MobData MOB = mobs::mobs.at(mob_id);
    ecs::TempEntityData e;
    e.name = MOB.name;
    e.components = {0,1,2,3,6,7,8,13};
    e.entity_type_id = ENTITY_TYPE_LIVE;
    e.x = x;
    e.y = y;
    e.z = 0.30;
    e.w = MOB.w;
    e.h = MOB.h;
    e.r = 0.45;
    e.g = 0.3;
    e.b = 0.3;
    e.a = 1.0;
    e.model_id = MOB.model_id;
    e.frame_id = 20101;
    e.side_id = ANIM_SIDE_FRONT;
    e.camera_type = CAMERA_DYNAMIC;
    e.mob_id = MOB.mob_id;

    // Create utils::random_from_range later
    e.dmg = MOB.max_dmg;
    e.speed = MOB.max_speed;
    e.hp = MOB.max_hp;
    e.def = MOB.max_def;

    e.animated = true;

    models::load(e.model_id);
    int mob_entity_id = ecs::create_entity(&e);
    return mob_entity_id;
  }

}
