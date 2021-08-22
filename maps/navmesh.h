#ifndef NAV_H
#define NAV_H

// will calculate the navmesh polygons here
// should take into consideration the map and other agents (npcs, other mobs)? or it should not
namespace nav 
{

  struct ConvexPolygon
  {
    // find edges of it?



  };


  // void calculate(std::vector<quads::Quad> tile_map)
  // {
  //   // keep array of solid/non solid tiles?
  // }

  struct SimpleQuad
  { 
    float x;
    float y;
    bool solid;
  };
  // std::vector<SimpleQuad> = {};



  void calculate_dev()
  {
    // make array










  }






}

#endif