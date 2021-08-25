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
  int MAX_TILE_ID;
  bool IN_POLYGON = false;
  int CURRENT_POLYGON_ID = 0;

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
    std::vector<NavTile> tiles;
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

    MAX_ROW = (vertex_height - 1);
    MAX_COL = (vertex_width - 1);
    MAX_TILE_ID = (MAX_COL*1000)+MAX_ROW;
    
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

  // check if given tile matches the range of any of already created polygons
  int get_polygon_id_if_belongs(NavTile t, std::map<int, NavPolygon> polygons)
  {
    int polygon_id = -1;
    for (auto const& cp : polygons)
    {
      if((t.x >= cp.second.min_x) & (t.x <= cp.second.max_x) & (t.y >= cp.second.min_y) & (t.y <= cp.second.max_y))
      {
        polygon_id = cp.first;
        break;
      }
    }
    return polygon_id;
  }


  // Analyze tiles and search for polygons 
  void define_polygons()
  {
    CURRENT_POLYGON_ID = 0;
    NavPolygons.clear();
    NavPolygon cp;
    cp.id = CURRENT_POLYGON_ID;
    for (auto const& t : NavTiles)
    {
      if(!t.second.checked){
        if(t.second.solid){
          if(t.first == MAX_TILE_ID){
            NavPolygons.insert({cp.id, cp});
          }
          continue;
        } else {
          // if not solid
          if(!IN_POLYGON){
            NavPolygon cp;
            CURRENT_POLYGON_ID += 1;
            cp.id = CURRENT_POLYGON_ID;
            cp.min_x=t.second.x;
            cp.min_y=t.second.y;
            IN_POLYGON = true;
            cp.tiles.push_back(t.second);
          } else {
            // already in polygon
            int poly_id = get_polygon_id_if_belongs(t.second, NavPolygons);
            if(poly_id > -1){
              NavPolygons[poly_id].tiles.push_back(t.second);

              if(t.second.tile_id_right > -1){
                if(NavTiles[t.second.tile_id_right].solid){
                  NavPolygons[poly_id].max_x = t.second.x;
                }
              }

              if(t.second.tile_id_down > -1){
                if(NavTiles[t.second.tile_id_down].solid){
                  NavPolygons[poly_id].max_y = t.second.y;
                }
              }
            } else {
              if ((t.second.x >= cp.min_x) & (t.second.x <= cp.max_x) & (t.second.y >= cp.min_y) & (t.second.y <= cp.max_y)){
                cp.tiles.push_back(t.second);
              } else {
                NavPolygons.insert({cp.id, cp});
                NavPolygon cp;
                CURRENT_POLYGON_ID += 1;
                cp.id = CURRENT_POLYGON_ID;
                cp.min_x=t.second.x;
                cp.min_y=t.second.y;
                cp.tiles.push_back(t.second);
              }

              // single column rule:
              if (t.second.tile_id_right > -1){
                int next_tile_polygon_id = -1;
                for (auto const& cp : NavPolygons)
                {
                  if((NavTiles[t.second.tile_id_right].x >= cp.second.min_x) & 
                     (NavTiles[t.second.tile_id_right].x <= cp.second.max_x) & 
                     (NavTiles[t.second.tile_id_right].y >= cp.second.min_y) & 
                     (NavTiles[t.second.tile_id_right].y <= cp.second.max_y))
                  {
                    next_tile_polygon_id = cp.first;
                    break;
                  }
                }
                if (next_tile_polygon_id > -1){
                  cp.max_x = t.second.x;
                }
              }

              // check neighbouring tiles:
              if (t.second.tile_id_right > -1){
                if(NavTiles[t.second.tile_id_right].solid){
                  cp.max_x = t.second.x;
                }
              } else {
                cp.max_x = t.second.x;
              }

              if (t.second.tile_id_down > -1){
                if(NavTiles[t.second.tile_id_down].solid){
                  cp.max_y = t.second.y;
                }
              } else {
                cp.max_y = t.second.y;
              }
            }
          }
        }
      }
      if(t.first == MAX_TILE_ID){
        NavPolygons.insert({cp.id, cp});
      }
    }
  }

  void print()
  {
    int polygon_array[MAX_ROW+1][MAX_COL+1] = {};
    std::cout << "Navpolygons size: " << NavPolygons.size() << std::endl;
    for (auto const& cp : NavPolygons)
    { 
      for(int t=0; t < cp.second.tiles.size(); t++){
        polygon_array[cp.second.tiles[t].y][cp.second.tiles[t].x] = cp.first;
      }
    }

    for(int r=0; r < (MAX_ROW+1); r++){
      for(int c=0; c < (MAX_COL+1); c++){
        std::cout << polygon_array[r][c] << " ";
      }
      std::cout << std::endl;
    }
  }

  
  void init(std::string map_name,
            int vertex_width, 
            int vertex_height)
  {
    nav::load_navtiles(map_name, vertex_width, vertex_height);
    nav::define_polygons();
    nav::print();
  }





}

#endif