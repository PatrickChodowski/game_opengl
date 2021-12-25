#include <fstream>
#include <vector>
#include <string>

#include "utils.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_NAVMESH_H
#define MODULES_NAVMESH_H

namespace nav 
{
  struct NavTileData
  {
    int x, y;
    int id;
    int tile_id_right;
    int tile_id_down;
    int tile_id_rightdown;
    int frame_id;
    float x_real, y_real;
    bool solid;
  };

  struct NavPolygonData
  {
    int id;
    float min_y;
    float min_x;
    float max_y;
    float max_x;
    std::vector<NavTileData> tiles;
  };

   // almost navigate, but no, its Nav Gate (gate between 2 polygons). Why I am not a comedian
  struct NavGateData 
  {
    int id;
    int a_id;
    int b_id;
    int orientation;

    float gate_min_x;
    float gate_max_x;
    float gate_min_y;
    float gate_max_y;
  };

  // NavNode store information about AABB box, scaled AABB box, edges(map) and tile count
  struct NavNodeData
  {
    float min_y;
    float min_x;
    float max_y;
    float max_x;
    int id;
    int count_tiles;
    phmap::flat_hash_map<int, NavGateData> edges;
  };

  extern phmap::flat_hash_map<int, NavNodeData> navnodes;
  extern phmap::flat_hash_map<int, NavPolygonData> navpolygons;
  extern phmap::flat_hash_map<int, NavTileData> navtiles;
  extern std::vector<std::vector<int>> navmesh;
  extern int MAX_ROW;
  extern int MAX_COL;
  extern int MAX_TILE_ID;

  // check if given tile matches the range of any of already created polygons
  int _get_polygon_id_if_belongs(nav::NavTileData t);

  // Load navtiles to navtiles map
  void _load_navtiles();

  // Takes navtiles and uses them to define navpolygons
  void _load_navpolygons();

  // function for testing if polygons intersect
  bool _aabb_intersect(nav::NavNodeData a, nav::NavNodeData b);

  // Takes navpolygons to create navnodes
  void _load_navnodes();

  // Finds edges between navnodes. Populates edges for each Node
  void _join_navnodes();

  // Loads the navmesh map
  void _load_navmesh();

  // Initialize the navmesh, nav polygons, graph
  void init(int map_id);

  // Clear navmesh data
  void clear();

  // Debug render the lines around polygons
  void render();



}

#endif
