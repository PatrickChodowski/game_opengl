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
    float x_real;
    float y_real;
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

  struct NavEdge
  {
    int a_id;
    int b_id;
    float x;
    float y; 
  };

  struct NavNode
  {
    int id;
    float min_y;
    float min_x;
    float max_y;
    float max_x;
    std::map<int, NavEdge> edges;
    int count_tiles;
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
          t.x_real = c * camera::tile_dim;
          t.y_real = r * camera::tile_dim;
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

    // if(t.id == 12009){
    //   std::cout << "12009 x : " << t.x << std::endl; // 2
    //   std::cout << "12009 y : " << t.y << std::endl; // 1
    //   // std::cout << "Polygon 1 min_x: " << polygons[1].min_x << std::endl;
    //   // std::cout << "Polygon 1 max_x: " << polygons[1].max_x << std::endl;
    //   // std::cout << "Polygon 1 min_y: " << polygons[1].min_y << std::endl;
    //   // std::cout << "Polygon 1 max_y: " << polygons[1].max_y << std::endl;
    // }

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
          //std::cout << " [1] Tile " << t.first << " belongs to " << poly_id << std::endl;

          //  // single column rule
          if (t.second.tile_id_right > -1){
            int next_tile_polygon_id = get_polygon_id_if_belongs(NavTiles[t.second.tile_id_right], NavPolygons);
            if (next_tile_polygon_id > -1 & next_tile_polygon_id < poly_id){
                //std::cout << "Netx tile polygon id is " << next_tile_polygon_id << std::endl;
                NavPolygons[poly_id].max_x = t.second.x;
                //std::cout << "Polygon (Sincle column rule)" << poly_id << " max_x updated to " << NavPolygons[poly_id].max_x << std::endl;
              }
          }

          if(t.second.tile_id_right > -1){
              if(NavTiles[t.second.tile_id_right].solid){
                NavPolygons[poly_id].max_x = t.second.x;
                //std::cout << "Polygon " << poly_id << " max_x updated to " << NavPolygons[poly_id].max_x << std::endl;
              }
          }

          if(t.second.tile_id_down > -1){
            if(NavTiles[t.second.tile_id_down].solid){
                NavPolygons[poly_id].max_y = t.second.y;
                //std::cout << "Polygon " << poly_id << " max_y updated to " << NavPolygons[poly_id].max_y << std::endl;
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
            int next_tile_polygon_id = get_polygon_id_if_belongs(NavTiles[t.second.tile_id_right], NavPolygons);
              if((NavTiles[t.second.tile_id_right].solid) | ((next_tile_polygon_id > -1 & next_tile_polygon_id < CP.id))){
                CP.max_x = t.second.x;
                //std::cout << "Polygon (solid tile_id_right or assigned)" << CP.id << " max_x updated to " << CP.max_x << std::endl;
              }
          } else {
              CP.max_x = t.second.x;
              //std::cout << "Polygon (empty tile_id_right)" << CP.id << " max_x updated to " << CP.max_x << std::endl;
          }

          if (t.second.tile_id_down > -1){
              if(NavTiles[t.second.tile_id_down].solid){
                CP.max_y = t.second.y;
                //std::cout << "Polygon (solid tile_id_down)" << CP.id << " max_y updated to " << CP.max_y << std::endl;
              }
          } else {
              CP.max_y = t.second.y;
              //std::cout << "Polygon (empty tile_id_down)" << CP.id << " max_y updated to " << CP.max_y << std::endl;
          }

          NavPolygons.insert({CP.id, CP});
          // std::cout << "Added Polygon ID " << CP.id << " to NavPolygons. Size: " <<  NavPolygons.size() << std::endl;
          // std::cout << " min_x: " << CP.min_x << std::endl;
          // std::cout << " max_x: " << CP.max_x << std::endl;
          // std::cout << " min_y: " << CP.min_y << std::endl;
          // std::cout << " max_y: " << CP.max_y << std::endl;
          // std::cout << " [0] Tile " << t.first << " belongs to " << CP.id << std::endl;
        }
      }
    }
    return NavPolygons;
  }
  





  void print_polygons(std::map<int, NavPolygon> nav_polygons,
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



  std::map<int, NavNode> init_nodes(std::map<int, NavPolygon> nav_polygons)
  {
    std::map<int, NavNode> nav_nodes;
    // convert polygon to node information
    for (auto const& cp : nav_polygons)
    { 
      NavNode node;
      node.id = cp.first;
      node.count_tiles = 0;

      float min_x = (float)INT_MAX;
      float max_x = 0.0f;
      float min_y = (float)INT_MAX;
      float max_y = 0.0f;
      std::cout << min_x << std::endl;

      // find min_, max_, x , y from tiles of each polygon
      for(int t=0; t < cp.second.tiles.size(); t++){

          // debug
          if(cp.first == 7){
            std::cout << "tile x : " << cp.second.tiles[t].x_real << std::endl;
            std::cout << "tile y : " << cp.second.tiles[t].x_real << std::endl;
          }

          node.count_tiles += 1;
          if(cp.second.tiles[t].x_real < min_x){
            min_x = cp.second.tiles[t].x_real;
          };

          if(cp.second.tiles[t].x_real > max_x){
            max_x = cp.second.tiles[t].x_real;
          };

          if(cp.second.tiles[t].y_real < min_y){
            min_y = cp.second.tiles[t].x_real;
          };

          if(cp.second.tiles[t].y_real > max_y){
            max_y = cp.second.tiles[t].y_real;
          };
      };
      node.min_x = min_x;
      node.max_x = max_x + camera::tile_dim; // as it needs the edge of the tile
      node.min_y = min_y;
      node.max_y = max_y + camera::tile_dim; // as it needs the edge of the tile
      nav_nodes.insert({node.id, node});

      std::cout << "Nav Node ID: " << node.id << std::endl;
      std::cout << "Nav Node min x: " << node.min_x << std::endl;
      std::cout << "Nav Node max x: " << node.max_x << std::endl;
      std::cout << "Nav Node min y: " << node.min_y << std::endl;
      std::cout << "Nav Node max y: " << node.max_y << std::endl;
      std::cout << "Nav Node tile count : " << node.count_tiles << std::endl;
    }
    std::cout << "Nav Nodes size: " << nav_nodes.size() << std::endl;
    return nav_nodes;
  }




  
  std::map<int, NavNode> init(std::string map_name,
                                 int vertex_width, 
                                 int vertex_height)
  {
    nav::load_navtiles(map_name, vertex_width, vertex_height);
    std::map<int, NavPolygon> nav_polygons = nav::define_polygons();
    nav::print_polygons(nav_polygons, vertex_width, vertex_height);

    std::map<int, NavNode> nav_nodes = nav::init_nodes(nav_polygons);
    return nav_nodes;
  }





}

#endif