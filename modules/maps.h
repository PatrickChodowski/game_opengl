#include <vector>
#include <map>

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

    bool is_clicked;
    bool is_solid;
  };

  extern std::vector<quad::QuadData> MapQuads;
  extern std::map<int, TileData> tiles;
  extern std::vector<int> UsedTileIds;









}


#endif