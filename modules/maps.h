#include <vector>
#include <map>
#include "quad.h"

#ifndef MODULES_MAPS_H
#define MODULES_MAPS_H

// Persistent data. Table of tiles and all operations on this table. Loaded on new level
namespace maps
{
  
  struct TileData
  {
    int tile_id;
    int tile_type_id;
    int texture_id;
    int frame_id;

    float x;
    float y;
    float h;
    float w;
    float mid_x;
    float mid_y;
    float diag;
  
    // scaled space
    float s_x;
    float s_y;
    float s_h;
    float s_w;
    float s_mid_x;
    float s_mid_y;
    float s_diag;

    bool is_camera_static;
    bool is_clicked;
    bool is_solid;

    // collision boxes
    std::map<int, collisions::AABB> abs;
  };

  extern std::vector<quad::QuadData> TileQuads;
  extern std::map<int, TileData> tiles;
  extern std::vector<int> UsedTileIds;









}


#endif