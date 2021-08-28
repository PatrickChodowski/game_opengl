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
      if((x >= nn.second.s_min_x) & (x <= nn.second.s_max_x) & (y >= nn.second.s_min_y) & (y <= nn.second.s_max_y))
      { 
        nav_node_id = nn.first;
        break;
      }
     }
    std::cout << " Point: " << x << "," << y << " Node id: " << nav_node_id << std::endl;
    return nav_node_id;
  }

int min_distance(std::vector<float> distances, 
                std::vector<bool> polygon_included,
                int polygon_count
                )
{
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < polygon_count; v++)
        if (polygon_included[v] == false && distances[v] <= min)
            min = distances[v], min_index = v;
 
    return min_index;
}

  void find_path_djikstra(int start_node_id, int end_node_id)
  {
    if((start_node_id > -1) & (end_node_id > -1))
    {
      int polygon_count = nav::NavMesh.size();
      std::vector<bool> polygon_included = {};
      std::vector<float> distances = {};

      for(int v = 0; v < polygon_count; v++)
      {
        polygon_included.push_back(false);
        distances.push_back(INT_MAX);
      }

      // distance of end_node_id to itself will be always 0:
      distances[(end_node_id-1)] = 0;

      for (int count = 0; count < polygon_count - 1; count++) 
      {
        int u = min_distance(distances, polygon_included, polygon_count);
        polygon_included[u] = true;

        for (int v = 0; v < polygon_count; v++)
        {
          if (!polygon_included[v] && nav::NavMeshGraph[u][v] && distances[u] != INT_MAX
              && distances[u] + nav::NavMeshGraph[u][v] < distances[v]){
                distances[v] = distances[u] + nav::NavMeshGraph[u][v];
          }
        }
      }

      for (int i = 0; i < polygon_count; i++)
      {
        std::cout  << (i+1) << " \t\t" <<distances[i] << std::endl;
      }
    }
  }
}






#endif
