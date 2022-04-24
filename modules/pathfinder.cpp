
#include <algorithm>
#include <climits>
#include <string>
#include <vector>


#include "navmesh.h"
#include "pathfinder.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace paths
{
  phmap::flat_hash_map<int, phmap::flat_hash_map<int, DjikstraStep>> PathMap;

  int get_navnode_id(float x, float y)
  {
    int nav_node_id = -1;
    for (auto const& nn : nav::navnodes)
    { 
      if((x >= nn.second.min_x) & (x <= nn.second.max_x) & (y >= nn.second.min_y) & (y <= nn.second.max_y))
      { 
        nav_node_id = nn.first;
        break;
      }
     }
    return nav_node_id;
  }

  int get_min_distance(std::vector<float> distances, 
                       std::vector<bool> polygon_included,
                       int polygon_count)
  {
    int min = INT_MAX, min_index;
    for (int v = 0; v < polygon_count; v++)
    {
      if (polygon_included[v] == false && distances[v] <= min)
      {
        min = distances[v], min_index = v;
      }
    }
    return min_index;
  }



  phmap::flat_hash_map<int, DjikstraStep> find_paths_djikstra(int node_id)
  {
    int polygon_count = nav::navnodes.size();
    std::vector<bool> polygon_included = {};
    std::vector<float> distances = {};
    std::vector<int> previous_nodes = {};

    for(int v = 0; v < polygon_count; v++)
    {
      polygon_included.push_back(false);
      distances.push_back(1000);
      previous_nodes.push_back(0);
    }

    // distance of end_node_id to itself will be always 0:
    distances[(node_id-1)] = 0;
    for (int count = 0; count < polygon_count - 1; count++) 
    {
      int u = get_min_distance(distances, polygon_included, polygon_count);
      polygon_included[u] = true;

      for (int v = 0; v < polygon_count; v++)
      {
        if (!polygon_included[v] && nav::navmesh[u][v] && distances[u] != 1000
            && distances[u] + nav::navmesh[u][v] < distances[v])
        {
              distances[v] = distances[u] + nav::navmesh[u][v];
              previous_nodes[v] = (u+1);
        }
      }
    }
    phmap::flat_hash_map<int, DjikstraStep> djikstra = {};
    for (int i = 0; i < polygon_count; i++)
    {
      DjikstraStep ds;
      ds.distance = distances[i];
      ds.previous_step = previous_nodes[i];
      djikstra.insert({(i+1),ds});
    }
    return djikstra;
  }

  void make_path_map()
  {
    paths::PathMap.clear();
    for (auto const& nn : nav::navnodes)
    { 
      phmap::flat_hash_map<int, DjikstraStep> djikstra = find_paths_djikstra(nn.first);
      paths::PathMap.insert({nn.first, djikstra});
    }
  }

  std::vector<int> find_path(int start_node_id, int end_node_id)
  {
    std::vector<int> path = {};
    if((start_node_id > -1) & (end_node_id > -1))
    {
      int next_distance;
      int next_node = end_node_id;
      path.push_back(next_node);
      next_distance = paths::PathMap[start_node_id][next_node].distance;

      while(next_distance > 1)
      {
        next_node = paths::PathMap[start_node_id][next_node].previous_step;
        next_distance = paths::PathMap[start_node_id][next_node].distance;
        path.push_back(next_node);
      }
      path.push_back(start_node_id);
      reverse(path.begin(), path.end());
    }
    return path;
  }


}
