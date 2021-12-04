
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "debug.h"
#include "game.h"
#include "maps.h"
#include "navmesh.h"
#include "utils.h"

#include "../dictionary.h"

namespace nav
{
  std::map<int, NavNodeData> navnodes = {};
  std::map<int, NavPolygonData> navpolygons = {};
  std::map<int, NavTileData> navtiles = {};
  std::vector<std::vector<int>> navmesh = {};
  int MAX_ROW;
  int MAX_COL;
  int MAX_TILE_ID;


  void _load_navtiles(int map_id)
  {
    nav::navtiles.clear();
    std::string file_path = "data/maps/" + maps::maps[map_id].name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());

    MAX_ROW = (maps::maps[map_id].vertex_height - 1);
    MAX_COL = (maps::maps[map_id].vertex_width - 1);
    MAX_TILE_ID = (MAX_COL*1000)+MAX_ROW;

    if (in_file.is_open())
    {
      for (int r = 0; r < maps::maps[map_id].vertex_height; r++)
      {
        for (int c = 0; c < maps::maps[map_id].vertex_width; c++)
        {
          struct nav::NavTileData t;
          t.x = c;
          t.y = r;
          t.x_real = c * maps::default_tile_width;
          t.y_real = r * maps::default_tile_height;
          in_file >> t.frame_id;

          if(t.frame_id > 10 && t.frame_id < 20){
            t.solid = true;
          } else {
            t.solid = false;
          }
          // unique id based on row and column indices
          t.id = (r*1000)+c;
          // std::cout << t.id <<std::endl;

          if((t.x < MAX_COL) & (t.y < MAX_ROW)){
            t.tile_id_right = (r*1000)+(c+1);
            t.tile_id_rightdown = ((r+1)*1000)+(c+1);
            t.tile_id_down = ((r+1)*1000)+(c);
          } else if ((t.x < MAX_COL) & (t.y == MAX_ROW)){
            t.tile_id_right = (r*1000)+(c+1);
            t.tile_id_rightdown = -1;
            t.tile_id_down = -1;
          } else if ((t.x == MAX_COL) & (t.y < MAX_ROW)){
            t.tile_id_right = -1;
            t.tile_id_rightdown = -1;
            t.tile_id_down = ((r+1)*1000)+(c);
          } else if ((t.x == MAX_COL) & (t.y < MAX_ROW)){
            t.tile_id_right = -1;
            t.tile_id_rightdown = -1;
            t.tile_id_down = -1;
          }
          navtiles.insert({t.id, t});
        }
      }
    }
  };

  int _get_polygon_id_if_belongs(nav::NavTileData t)
  {
    int polygon_id = -1;
    for (auto const& cp : nav::navpolygons)
    {
      if((t.x >= cp.second.min_x) & (t.x <= cp.second.max_x) & (t.y >= cp.second.min_y) & (t.y <= cp.second.max_y))
      {
        polygon_id = cp.first;
        break;
      }
    }
    return polygon_id;
  };

  void _load_navpolygons()
  {
    nav::navpolygons.clear();
    int CURRENT_POLYGON_ID = 0;
    for (auto const& t : nav::navtiles)
    {
      if(t.second.solid){
        continue;
      } else {
        int poly_id = nav::_get_polygon_id_if_belongs(t.second);
        if(poly_id > -1)
        {
          nav::navpolygons[poly_id].tiles.push_back(t.second);
          //  single column rule
          if (t.second.tile_id_right > -1)
          {
            int next_tile_polygon_id = nav::_get_polygon_id_if_belongs(nav::navtiles[t.second.tile_id_right]);
            if (next_tile_polygon_id > -1 & next_tile_polygon_id < poly_id)
            {
              nav::navpolygons[poly_id].max_x = t.second.x;
            }
          }

          if(t.second.tile_id_right > -1)
          {
            if(nav::navtiles[t.second.tile_id_right].solid)
            {
              nav::navpolygons[poly_id].max_x = t.second.x;
            }
          }
          if(t.second.tile_id_down > -1)
          {
            if(nav::navtiles[t.second.tile_id_down].solid)
            {
              nav::navpolygons[poly_id].max_y = t.second.y;
            }
          }
        } else {
          nav::NavPolygonData CP;
          CURRENT_POLYGON_ID += 1;
          CP.id = CURRENT_POLYGON_ID;
          CP.min_x=t.second.x;
          CP.min_y=t.second.y;
          CP.max_x=nav::MAX_COL;
          CP.max_y=nav::MAX_ROW;
          CP.tiles.push_back(t.second);

          if (t.second.tile_id_right > -1)
          {
            int next_tile_polygon_id = nav::_get_polygon_id_if_belongs(nav::navtiles[t.second.tile_id_right]);
            if((nav::navtiles[t.second.tile_id_right].solid) | ((next_tile_polygon_id > -1 & next_tile_polygon_id < CP.id)))
            {
              CP.max_x = t.second.x;
            }
          } else 
          {
            CP.max_x = t.second.x;
          }

          if (t.second.tile_id_down > -1)
          {
            if(nav::navtiles[t.second.tile_id_down].solid)
            {
              CP.max_y = t.second.y;
            }
          } else 
          {
            CP.max_y = t.second.y;
          }

          nav::navpolygons.insert({CP.id, CP});
        }
      }
    }
  }

  bool _aabb_intersect(nav::NavNodeData a, nav::NavNodeData b)
  {
    return ((a.min_x <= b.max_x & a.max_x >= b.min_x) & (a.min_y <= b.max_y & a.max_y >= b.min_y));
  }

  void _load_navnodes()
  {
    nav::navnodes.clear();
    for (auto const& cp : nav::navpolygons)
    { 
      nav::NavNodeData node;
      node.id = cp.first;
      node.count_tiles = 0;
      node.min_x = cp.second.min_x * maps::default_tile_width;
      node.max_x = (cp.second.max_x * maps::default_tile_width) + maps::default_tile_width;
      node.min_y = cp.second.min_y * maps::default_tile_height;
      node.max_y = (cp.second.max_y * maps::default_tile_height) + maps::default_tile_height;
      for(int t=0; t < cp.second.tiles.size(); t++)
      {
        node.count_tiles += 1;
      }
      nav::navnodes.insert({node.id, node});
    }
  }

  void _join_navnodes()
  {
    // for each node iterate and find nodes colliding (or rather touching each other)
    // greate NavGate for each connection
    // Polygon_id, Gate =  edge

    for (auto & nn : nav::navnodes)
    { 
      std::map<int, NavGateData> _edges = {};
      for (auto const& t : nav::navnodes)
      { 
        if(nn.first != t.first){
          if(nav::_aabb_intersect(nn.second, t.second))
          {
            nav::NavGateData ng;
            ng.a_id = nn.first;
            ng.b_id = t.first;
            ng.id = (nn.first*1000) + t.first;

            ng.gate_min_x = std::max(nn.second.min_x, t.second.min_x);
            ng.gate_max_x = std::min(nn.second.max_x, t.second.max_x);
            ng.gate_min_y = std::max(nn.second.min_y, t.second.min_y);
            ng.gate_max_y = std::min(nn.second.max_y, t.second.max_y);

            if(ng.gate_min_x == ng.gate_max_x)
            {
              ng.orientation = NAVGATE_VERTICAL_ORIENTATION;
            } else if(ng.gate_min_y == ng.gate_max_y)
            {
              ng.orientation = NAVGATE_HORIZONTAL_ORIENTATION;
            }
            _edges.insert({t.first, ng});       
          }
        }
      }
      nav::navnodes[nn.first].edges = _edges;
    }
  }


  void _load_navmesh()
  {
    int value;
    int polygon_count = nav::navnodes.size();
    nav::navmesh.clear();
    for(int a = 0; a < polygon_count; a++)
    {
      std::vector<int> row = {};
      for(int b = 0; b < polygon_count; b++)
      {
        if(nav::navnodes[(a+1)].edges.count((b+1)) > 0){
          value = 1;
        } else {
          value = 0;
        }
        row.push_back(value);
      }
      nav::navmesh.push_back(row);
    }
  }

  void init(int map_id)
  {
    nav::_load_navtiles(map_id);
    nav::_load_navpolygons();
    nav::_load_navnodes();
    nav::_join_navnodes();
    nav::_load_navmesh();

  };

  void clear()
  {
    nav::navnodes.clear();
    nav::navpolygons.clear();
    nav::navtiles.clear();
    nav::navmesh.clear();
    MAX_ROW = 0;
    MAX_COL = 0;
    MAX_TILE_ID = 0;
  }


  void render()
  {
    //std::cout << "nav polygons size: " << nav::navpolygons.size() << std::endl;
    if(game::IS_DEBUG_MODE)
    {
      for (auto const& [k, v] : nav::navnodes)
      {
        float w = v.max_x - v.min_x;
        float h = v.max_y - v.min_y;
        debug::render_square(v.min_x, v.min_y, w, h, 0.0, 1.0, 0.0, 1.0);

      } 
    }
  }


}
