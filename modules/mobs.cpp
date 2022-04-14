

#include "maps.h"
#include "mobs.h"
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

  void spawn(int mob_id, float x, float y)
  {
   
  }

}
