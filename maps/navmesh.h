#ifndef NAV_H
#define NAV_H

// will calculate the navmesh polygons here
// should take into consideration the map and other agents (npcs, other mobs)? or it should not
// created prototype algorithm in python and I have never felt smarter in my life
// big brain patrick
namespace nav 
{

  int MAX_ROW;
  int MAX_COL;
  bool IN_POLYGON = false;

  struct NavTile
  {
    int id;
    int x;
    int y;
    bool checked;
    bool solid;
    int tile_id_right;
    int tile_id_down;
    int tile_id_rightdown;
    int frame_id;
  };

  struct NavPolygon
  {
    int id;
    std::vector<NavTile> nav_tiles;
    int min_y;
    int min_x;
    int max_y = MAX_COL;
    int max_x = MAX_ROW;
  };


  std::map<int, NavTile> NavTiles = {};
  std::map<int, NavPolygon> NavPolygons = {};


  // step 1: propagate NavTiles map 
  void load_navtiles(std::string map_name,
                     int vertex_width, 
                     int vertex_height)
  {
    NavTiles.clear();
    std::string file_path = "maps/" + map_name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());

    if (in_file.is_open())
    {
      for (int r = 0; r < vertex_height; r++)
      {
        for (int c = 0; c < vertex_width; c++)
        {
          struct nav::NavTile t;
          t.x = c;
          t.y = r;
          t.checked = false;
          in_file >> t.frame_id;

          if(t.frame_id > 10 && t.frame_id < 20){
            t.solid = true;
          } else {
            t.solid = false;
          }
          // unique id based on row and column indices
          t.id = (c*1000)+r;
          // std::cout << t.id <<std::endl;

          if((t.x < MAX_COL) & (t.y < MAX_ROW)){
            t.tile_id_right = ((c+1)*1000)+r;
            t.tile_id_rightdown = ((c+1)*1000)+(r+1);
            t.tile_id_down = (c*1000)+(r+1);
          } else if ((t.x < MAX_COL) & (t.y == MAX_ROW)){
            t.tile_id_right = ((c+1)*1000)+r;
            t.tile_id_rightdown = -1;
            t.tile_id_down = -1;
          } else if ((t.x == MAX_COL) & (t.y < MAX_ROW)){
            t.tile_id_right = -1;
            t.tile_id_rightdown = -1;
            t.tile_id_down = (c*1000)+(r+1);
          } else if ((t.x == MAX_COL) & (t.y < MAX_ROW)){
            t.tile_id_right = -1;
            t.tile_id_rightdown = -1;
            t.tile_id_down = -1;
          }
          NavTiles.insert({t.id, t});
        }
      }
    }
  }
  






}

#endif