
#include <map>
#include <string>
#include <vector>

#ifndef MODULES_PATHFINDER_H
#define MODULES_PATHFINDER_H

namespace paths
{
  struct DjikstraStep
  {
    int distance;
    int previous_step;
  };

  extern std::map<int, std::map<int, DjikstraStep>> PathMap;


  int get_min_distance(std::vector<float> distances, 
                       std::vector<bool> polygon_included,
                       int polygon_count);

  std::map<int, DjikstraStep> find_paths_djikstra(int node_id);

  void make_path_map();

  std::vector<int> find_path(int start_node_id, int end_node_id);

}

#endif
