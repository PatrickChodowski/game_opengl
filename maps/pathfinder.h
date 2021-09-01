#ifndef PATHFINDER_H
#define PATHFINDER_H


// Depends on nav::navmesh created from nav::init
namespace paths
{
  int get_navnode_id(float x, float y)
  {
    int nav_node_id = -1;
    for (auto const& nn : nav::NavMesh)
    { 
      if((x >= nn.second.min_x) & (x <= nn.second.max_x) & (y >= nn.second.min_y) & (y <= nn.second.max_y))
      { 
        nav_node_id = nn.first;
        break;
      }
     }
    return nav_node_id;
  }

  int min_distance(std::vector<float> distances, 
                   std::vector<bool> polygon_included,
                   int polygon_count)
  {
      // Initialize min value
      int min = INT_MAX, min_index;
  
      for (int v = 0; v < polygon_count; v++)
          if (polygon_included[v] == false && distances[v] <= min)
              min = distances[v], min_index = v;
  
      return min_index;
  }

  struct DjikstraStep
  {
    int distance;
    int previous_step;
  };

  // node_id, target, <distance, previous_step>
  std::map<int, std::map<int, DjikstraStep>> PathMap;

  std::map<int, DjikstraStep> find_paths_djikstra(int node_id)
  {
    int polygon_count = nav::NavMesh.size();
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
      int u = min_distance(distances, polygon_included, polygon_count);
      polygon_included[u] = true;

      for (int v = 0; v < polygon_count; v++)
      {
        if (!polygon_included[v] && nav::NavMeshGraph[u][v] && distances[u] != 1000
            && distances[u] + nav::NavMeshGraph[u][v] < distances[v]){
              distances[v] = distances[u] + nav::NavMeshGraph[u][v];
              previous_nodes[v] = (u+1);
        }
      }
    }
    std::map<int, DjikstraStep> djikstra = {};
    for (int i = 0; i < polygon_count; i++)
    {
      DjikstraStep ds;
      // ds.target = (i+1);
      ds.distance = distances[i];
      ds.previous_step = previous_nodes[i];
      djikstra.insert({(i+1),ds});
      //std::cout  << (i+1) << " \t\t" << distances[i] << " \t\t" << previous_nodes[i] << std::endl;
    }
    return djikstra;
  }

  void make_path_map()
  {
    paths::PathMap.clear();
    for (auto const& nn : nav::NavMesh)
    { 
      std::map<int, DjikstraStep> djikstra = find_paths_djikstra(nn.first);
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

      for(int i=0; i < path.size(); i++)
      {
        std::cout << path[i] << " "; 
      }
      std::cout << std::endl;
    }
    return path;
  }
}

#endif
