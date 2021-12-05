
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

  //Get nav_node_id based on the X,Y position
  int get_navnode_id(float x, float y);

  // Get shortest distance between navnodes
  int get_min_distance(std::vector<float> distances, 
                       std::vector<bool> polygon_included,
                       int polygon_count);

  //Find paths from nav_node to all other navnodes using djikstra algorithm. 
  // Results is saved to PathMap
  std::map<int, DjikstraStep> find_paths_djikstra(int node_id);

  // Fills out the PathMap based on the navmesh graph
  void make_path_map();

  // Find path between start node and end node using PathMap
  std::vector<int> find_path(int start_node_id, int end_node_id);

}

#endif
