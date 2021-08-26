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

  struct NavTile
  {
    int id;
    int x;
    int y;
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
    int max_y;
    int max_x;
  };


  std::map<int, NavTile> NavTiles = {};
 
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

    if(t.id == 2001){
      std::cout << "2001 x : " << t.x << std::endl; // 2
      std::cout << "2001 y : " << t.y << std::endl; // 1
      std::cout << "Polygon 1 min_x: " << polygons[1].min_x << std::endl;
      std::cout << "Polygon 1 max_x: " << polygons[1].max_x << std::endl;
      std::cout << "Polygon 1 min_y: " << polygons[1].min_y << std::endl;
      std::cout << "Polygon 1 max_y: " << polygons[1].max_y << std::endl;
    }

    return polygon_id;
  }

  // Analyze tiles and search for polygons 
  std::map<int, NavPolygon> define_polygons()
  {
    int CURRENT_POLYGON_ID = 0;
    std::map<int, NavPolygon> NavPolygons = {};

    for (auto const& t : NavTiles)
    {
      // std::cout << "Assigning tile id: " << t.first << std::endl;
      if(t.second.solid){
        continue;
      } else {
        int poly_id = nav::get_polygon_id_if_belongs(t.second, NavPolygons);
        if(poly_id > -1){
          NavPolygons[poly_id].tiles.push_back(t.second);
          std::cout << " [1] Tile " << t.first << " belongs to " << poly_id << std::endl;

          if(t.second.tile_id_right > -1){
              if(NavTiles[t.second.tile_id_right].solid){
                NavPolygons[poly_id].max_x = t.second.x;
                std::cout << "Polygon " << poly_id << " max_x updated to " << NavPolygons[poly_id].max_x << std::endl;
              }
          }

          if(t.second.tile_id_down > -1){
            if(NavTiles[t.second.tile_id_down].solid){
                NavPolygons[poly_id].max_y = t.second.y;
                std::cout << "Polygon " << poly_id << " max_y updated to " << NavPolygons[poly_id].max_y << std::endl;
            }
          }

        } else {
          NavPolygon CP;
          CURRENT_POLYGON_ID += 1;
          CP.id = CURRENT_POLYGON_ID;
          CP.min_x=t.second.x;
          CP.min_y=t.second.y;
          CP.max_x=nav::MAX_COL;
          CP.max_y=nav::MAX_ROW;
          CP.tiles.push_back(t.second);

          if (t.second.tile_id_right > -1){
            int next_tile_polygon_id = -1;
            for (auto const& p : NavPolygons)
              {
                if((NavTiles[t.second.tile_id_right].x >= p.second.min_x) & 
                   (NavTiles[t.second.tile_id_right].x <= p.second.max_x) & 
                   (NavTiles[t.second.tile_id_right].y >= p.second.min_y) & 
                   (NavTiles[t.second.tile_id_right].y <= p.second.max_y))
                {
                  next_tile_polygon_id = p.first;
                  break;
                }
              }
              if (next_tile_polygon_id > -1){
                std::cout << "Netx tile polygon id is " << next_tile_polygon_id << std::endl;
                CP.max_x = t.second.x;
                std::cout << "Polygon (Sincle column rule)" << CP.id << " max_x updated to " << CP.max_x << std::endl;
              }
          }
          
          // check neighbouring tiles:
          if (t.second.tile_id_right > -1){
              if(NavTiles[t.second.tile_id_right].solid){
                CP.max_x = t.second.x;
                std::cout << "Polygon (solid tile_id_right)" << CP.id << " max_x updated to " << CP.max_x << std::endl;
              }
          } else {
              CP.max_x = t.second.x;
              std::cout << "Polygon (empty tile_id_right)" << CP.id << " max_x updated to " << CP.max_x << std::endl;
          }

          if (t.second.tile_id_down > -1){
              if(NavTiles[t.second.tile_id_down].solid){
                CP.max_y = t.second.y;
                std::cout << "Polygon (solid tile_id_down)" << CP.id << " max_y updated to " << CP.max_y << std::endl;
              }
          } else {
              CP.max_y = t.second.y;
              std::cout << "Polygon (empty tile_id_down)" << CP.id << " max_y updated to " << CP.max_y << std::endl;
          }

          NavPolygons.insert({CP.id, CP});
          std::cout << "Added Polygon ID " << CP.id << " to NavPolygons. Size: " <<  NavPolygons.size() << std::endl;
          std::cout << " min_x: " << CP.min_x << std::endl;
          std::cout << " max_x: " << CP.max_x << std::endl;
          std::cout << " min_y: " << CP.min_y << std::endl;
          std::cout << " max_y: " << CP.max_y << std::endl;
          std::cout << " [0] Tile " << t.first << " belongs to " << CP.id << std::endl;
        }
      }
    }
    return NavPolygons;
  }
  





  void print(std::map<int, NavPolygon> nav_polygons,
            int vertex_width, 
            int vertex_height)
  {
    // int polygon_array[vertex_height+1][vertex_width+1] = {};
    std::cout << "Navpolygons size: " << nav_polygons.size() << std::endl;

    // init rows:
    std::vector<std::vector<int>> polygon_array(vertex_height+1);

    // for each row create columns:
    for(int r=0; r < (vertex_height+1); r++){
      std::vector<int> cols(vertex_width+1);
      polygon_array[r] = cols;
    }

    for (auto const& cp : nav_polygons)
    { 
      for(int t=0; t < cp.second.tiles.size(); t++){
        polygon_array[cp.second.tiles[t].y][cp.second.tiles[t].x] = cp.first;
      }
    }

    for(int r=0; r < (nav::MAX_ROW+1); r++){
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
    std::map<int, NavPolygon> nav_polygons = nav::define_polygons();
    nav::print(nav_polygons, vertex_width, vertex_height);
  }





}

#endif