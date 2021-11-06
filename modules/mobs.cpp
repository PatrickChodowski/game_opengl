
#include "entity.h"
#include "maps.h"
#include "mobs.h"
#include "quads.h"
#include "utils.h"

#include "../dictionary.h"

#include <map>
#include <iostream>
#include <string>
#include <vector>

namespace mobs2
{

  std::map<int, MobData> mobs = {};

  void read_data(std::string name)
  {
    mobs2::MobData MD;
    std::string data_path = "./data/mobs/"+name+".json";
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    mobs2::mobs.insert({MD.id, MD});
  };


  void init()
  {
    std::vector<std::string> mob_list = utils2::list_json_files("./data/mobs/");
    for(int m=0; m < mob_list.size(); m++)
    {
      mobs2::read_data(mob_list[m]);
    }
  };

  void drop()
  {
    mobs2::mobs.clear();
  }


  void spawn(int map_id)
  {
    int MOB_ID = 0;
    // unpack each nest
    for(int i=0; i < maps2::maps[map_id].nests.size(); i++)
    {
      // for each mob of the nest
      for(int m=0; m<maps2::maps[map_id].nests[i].n; m++)
      {
        mobs2::MobData mdd = mobs2::mobs[MOB_ID];
        mdd.x = maps2::maps[map_id].nests[i].x;
        mdd.y = maps2::maps[map_id].nests[i].y;
        mdd.current_frame = 0;
        entity::create(mdd, OBJECT_TYPE_ENTITY, CAMERA_STATIC);
      }
    }
  };










}
