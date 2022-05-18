#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "maps2.h"
#include "models.h"
#include "navmesh.h"
#include "pathfinder.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace maps2
{
  phmap::flat_hash_map<int, maps2::MapData> maps = {};

  void init()
  {
    std::vector<std::string> maps_list = utils::list_json_files("data/maps2");
    for(int m=0; m < maps_list.size(); m++)
    {
      maps2::read_data(maps_list[m]);
    }
    std::cout << " [Maps2] Initialized" << std::endl;
  }

  void read_data(std::string name)
  {
    std::string data_path = "./data/maps2/"+name+".json";
    maps2::MapData MD;
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    maps2::maps.insert({MD.map_id, MD});
  };




}
