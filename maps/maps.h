#ifndef MAPS_H
#define MAPS_H


namespace maps 
{
  struct MapCache
  {
   int level_id;
   int map_id;

   // std::vector<mobs::Mob> level_mobs;
   // std::vector<items::Item> level_items_on_ground;
   std::vector<int> required_textures;
  };

  
  void read_map_data(std::string name)
  {
    /*
      Read map data reads the json data by map name and stores it inside maps::Catalog
    */    
    MapData MD;
    std::string data_path = "./maps/data/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);

    // add to catalog
    Catalog.insert({MD.id, MD});

    if(LOGGING == 0)
    { 
      std::cout << "Read-in map id: " << MD.id << ", name: " << MD.name << ", vertex_width: " <<
       MD.vertex_width << ", vertex_height: " << MD.vertex_height << ", texture_id: " << MD.texture_id << 
       ", default_player_x: " << MD.default_player_x << ", default_player_y: " << MD.default_player_y << 
       ",  door count: " << MD.doors.size() << std::endl;

      for(int d=0; d<MD.doors.size(); d++)
      {
       std::cout <<  "door_id: " << MD.doors[d].door_id << ", x: " 
       <<  MD.doors[d].x << ", y: "<< MD.doors[d].y << ", dest_map_id: " 
       << MD.doors[d].dest_map_id << ", player_enter_x: "<< MD.doors[d].player_enter_x << ", player_enter_y: " << MD.doors[d].player_enter_y << std::endl;
      }
    }
  }

  void init()
  { 
    /*
      Maps init just reads the list of maps from maps/data/ and for each map there
      performs read_map_data function - which means reading the json file into the 
      MapData object and store it inside maps::Catalog
    */

    logger::print("maps::init(): READING MAPS");
    std::vector<std::string> maps_list = utils::list_files("maps/data/");
    for(int m=0; m < maps_list.size(); m++)
    {
      read_map_data(maps_list[m]);
    }
  }


  std::vector<quads::Quad> load_map_from_file(std::string map_name, 
                                              int vertex_width, 
                                              int vertex_height, 
                                              int texture_id)

  {
    /*
      Loads map quads from map file
    */
    std::vector<quads::Quad> tile_map = {};
    std::string file_path = "maps/" + map_name;
    std::ifstream in_file;
    in_file.open(file_path.c_str());

    int n_tiles = vertex_width*vertex_height;

    // yes no solid array rowsxcols
    bool solid_array[vertex_height][vertex_width];

      // read in the tile info
    if (in_file.is_open())
    {
      for (int r = 0; r < vertex_height; r++)
        {
          for (int c = 0; c < vertex_width; c++)
          {
            struct quads::Quad quad;
            quad.id = quads::gen_quad_id();
            quad.x = c * camera::tile_dim;
            quad.y = r * camera::tile_dim;
            quad.w = camera::tile_dim;
            quad.h = camera::tile_dim;
            in_file >> quad.frame_id; // value of tile in text file
            quad.s_x = c * camera::tile_dim;
            quad.s_y = r * camera::tile_dim;
            quad.s_w = camera::tile_dim;
            quad.s_h = camera::tile_dim;
  
            quad.texture_id = texture_id;
            quad.is_clicked = 0.0;
            quad.type_id = QUAD_TYPE_MAP;
            quad.is_static = 0.0f;

            quad.solid = false;
            quad.coll = false;
            solid_array[r][c] = false;

            // if frame_id is between 10 and 20, then its solid
            if(quad.frame_id > 10 && quad.frame_id < 20){
              // 11-19
              quad.solid = true;
              quad.coll = true;
              solid_array[r][c] = true;
            } 

            quad.entity_type_id = ENTITY_TYPE_ID_NA;
            quad.alive = false;
            quad.r_col = 0.5;
            quad.g_col = 0.5;
            quad.b_col = 0.5;
            quad.a_col = 0.5;

            tile_map.push_back(quad);
          };
        } 
    }
    in_file.close();

    // // save solid array to file to file 
    // std::string arra_log_file_path = "./logs/map_solid_array.txt";
    // std::ofstream array_file (arra_log_file_path.c_str());
    // if (array_file.is_open())
    // {
    //   for(int i = 0; i < vertex_height; i ++)
    //   {
    //     for(int j = 0; j < vertex_width; j++)
    //     {
    //       array_file << solid_array[i][j] << " ";
    //     }
    //     array_file << "\n";
    //   }
    //   array_file.close();
    // }

    // // calculate navmesh polygons
    // struct ConvexPolygon
    // {
    //   int id;
    //   std::vector<int> tiles;
    // };

    // ConvexPolygon cp;
    // bool in_polygon = false;

    // for(int i = 0; i < vertex_height; i ++)
    //   {
    //     for(int j = 0; j < vertex_width; j++)
    //     {
    //       if(!solid_array[i][j]){
    //         // if not solid == if traversable
    //         if(in_polygon)
    //         {
    //           // if we are currently in polygon
    //           cp.tiles.push_back()

    //         }



    //       }











    return tile_map;
  }

  // map_id
  // default player x, y
  // it is supposed to load everything for given level
  // also needs some  cache struct data for mobs situation, items on the ground etc.
  // load needed textures, load vertices etc.
  void init_map(int map_id, int player_x, int player_y)
  {
    // Load quads for the map first
    std::cout << "Initializing map id: " << map_id
              << " Map name: " << maps::Catalog[map_id].name
              << " Vertex width: " << maps::Catalog[map_id].vertex_width
              << " Vertex height: " << maps::Catalog[map_id].vertex_height
              << " Map texture id: " << maps::Catalog[map_id].texture_id << std::endl;

    maps::MapQuads = load_map_from_file(maps::Catalog[map_id].name, 
                                        maps::Catalog[map_id].vertex_width, 
                                        maps::Catalog[map_id].vertex_height, 
                                        maps::Catalog[map_id].texture_id);
    
    std::map<int, nav::NavPolygon> navmesh = nav::init(maps::Catalog[map_id].name, 
                                                       maps::Catalog[map_id].vertex_width, 
                                                       maps::Catalog[map_id].vertex_height);

    for(int i = 0; i < maps::MapQuads.size(); i++)
    { 
      maps::MapQuads[i].a = quads::gen_vertex_id();
      maps::MapQuads[i].b = quads::gen_vertex_id();
      maps::MapQuads[i].c = quads::gen_vertex_id();
      maps::MapQuads[i].d = quads::gen_vertex_id();

      float norm_x_start = (float)textures::Catalog[maps::MapQuads[i].texture_id].frames[maps::MapQuads[i].frame_id].x/
      (float)textures::Catalog[maps::MapQuads[i].texture_id].width;

      float norm_x_end =  (float)(textures::Catalog[maps::MapQuads[i].texture_id].frames[maps::MapQuads[i].frame_id].x + 
      textures::Catalog[maps::MapQuads[i].texture_id].frames[maps::MapQuads[i].frame_id].w)/
      (float)textures::Catalog[maps::MapQuads[i].texture_id].width;

      // create vertex struct - A
      maps::MapQuads[i].v_a.vertex_id = maps::MapQuads[i].a;
      maps::MapQuads[i].v_a.tile_id = maps::MapQuads[i].id;
      maps::MapQuads[i].v_a.frame_id = maps::MapQuads[i].frame_id;
      maps::MapQuads[i].v_a.x_pos = (float)maps::MapQuads[i].x;
      maps::MapQuads[i].v_a.y_pos = (float)maps::MapQuads[i].y;
      maps::MapQuads[i].v_a.z_pos = 0.0f;
      maps::MapQuads[i].v_a.r_col = 0.0f;
      maps::MapQuads[i].v_a.g_col = 0.0f;
      maps::MapQuads[i].v_a.b_col = 0.0f;
      maps::MapQuads[i].v_a.a_col = 1.0f;
      maps::MapQuads[i].v_a.tex_coord_x = norm_x_start;
      maps::MapQuads[i].v_a.tex_coord_y = 0.0f;
      maps::MapQuads[i].v_a.texture_id = textures::Catalog[maps::MapQuads[i].texture_id].opengl_texture_id;
      maps::MapQuads[i].v_a.is_clicked = (float)maps::MapQuads[i].is_clicked;
      maps::MapQuads[i].v_a.type_id = (float)maps::MapQuads[i].type_id;
      maps::MapQuads[i].v_a.is_static = (float)maps::MapQuads[i].is_static;

      // create vertex struct - B
      maps::MapQuads[i].v_b.vertex_id = maps::MapQuads[i].b;
      maps::MapQuads[i].v_b.tile_id = maps::MapQuads[i].id;
      maps::MapQuads[i].v_b.frame_id = maps::MapQuads[i].frame_id;
      maps::MapQuads[i].v_b.x_pos = (float)maps::MapQuads[i].x + (float)TILE_DIM  - (float)quads::VERTEX_OFFSET;
      maps::MapQuads[i].v_b.y_pos = (float)maps::MapQuads[i].y;
      maps::MapQuads[i].v_b.z_pos = 0.0f;
      maps::MapQuads[i].v_b.r_col = 0.0f;
      maps::MapQuads[i].v_b.g_col = 0.0f;
      maps::MapQuads[i].v_b.b_col = 0.0f;
      maps::MapQuads[i].v_b.a_col = 1.0f;
      maps::MapQuads[i].v_b.tex_coord_x = norm_x_end;
      maps::MapQuads[i].v_b.tex_coord_y = 0.0f;
      maps::MapQuads[i].v_b.texture_id = textures::Catalog[maps::MapQuads[i].texture_id].opengl_texture_id;
      maps::MapQuads[i].v_b.is_clicked = (float)maps::MapQuads[i].is_clicked;
      maps::MapQuads[i].v_b.type_id = (float)maps::MapQuads[i].type_id;
      maps::MapQuads[i].v_b.is_static = (float)maps::MapQuads[i].is_static;

      // create vertex struct - C
      maps::MapQuads[i].v_c.vertex_id = maps::MapQuads[i].c;
      maps::MapQuads[i].v_c.tile_id = maps::MapQuads[i].id;
      maps::MapQuads[i].v_c.frame_id = maps::MapQuads[i].frame_id;
      maps::MapQuads[i].v_c.x_pos = (float)maps::MapQuads[i].x;
      maps::MapQuads[i].v_c.y_pos = (float)maps::MapQuads[i].y + (float)TILE_DIM - (float)quads::VERTEX_OFFSET;
      maps::MapQuads[i].v_c.z_pos = 0.0f;
      maps::MapQuads[i].v_c.r_col = 0.0f;
      maps::MapQuads[i].v_c.g_col = 0.0f;
      maps::MapQuads[i].v_c.b_col = 0.0f;
      maps::MapQuads[i].v_c.a_col = 1.0f;
      maps::MapQuads[i].v_c.tex_coord_x = norm_x_start;
      maps::MapQuads[i].v_c.tex_coord_y = 1.0f;
      maps::MapQuads[i].v_c.texture_id = textures::Catalog[maps::MapQuads[i].texture_id].opengl_texture_id;
      maps::MapQuads[i].v_c.is_clicked = (float)maps::MapQuads[i].is_clicked;
      maps::MapQuads[i].v_c.type_id = (float)maps::MapQuads[i].type_id;
      maps::MapQuads[i].v_c.is_static = (float)maps::MapQuads[i].is_static;


      // create vertex struct - D
      maps::MapQuads[i].v_d.vertex_id = maps::MapQuads[i].d;
      maps::MapQuads[i].v_d.tile_id = maps::MapQuads[i].id;
      maps::MapQuads[i].v_d.frame_id = maps::MapQuads[i].frame_id;
      maps::MapQuads[i].v_d.x_pos = (float)maps::MapQuads[i].x + (float)TILE_DIM  - (float)quads::VERTEX_OFFSET;
      maps::MapQuads[i].v_d.y_pos = (float)maps::MapQuads[i].y + (float)TILE_DIM - (float)quads::VERTEX_OFFSET;
      maps::MapQuads[i].v_d.z_pos = 0.0f;
      maps::MapQuads[i].v_d.r_col = 1.0f;
      maps::MapQuads[i].v_d.g_col = 1.0f;
      maps::MapQuads[i].v_d.b_col = 1.0f;
      maps::MapQuads[i].v_d.a_col = 1.0f;
      maps::MapQuads[i].v_d.tex_coord_x = norm_x_end;
      maps::MapQuads[i].v_d.tex_coord_y = 1.0f;
      maps::MapQuads[i].v_d.texture_id = textures::Catalog[maps::MapQuads[i].texture_id].opengl_texture_id;
      maps::MapQuads[i].v_d.is_clicked = (float)maps::MapQuads[i].is_clicked;
      maps::MapQuads[i].v_d.type_id = (float)maps::MapQuads[i].type_id;
      maps::MapQuads[i].v_d.is_static = (float)maps::MapQuads[i].is_static;

      // create vindices 
      maps::MapQuads[i].i_left.a = maps::MapQuads[i].a;
      maps::MapQuads[i].i_left.b = maps::MapQuads[i].b;
      maps::MapQuads[i].i_left.c = maps::MapQuads[i].c;

      maps::MapQuads[i].i_right.a = maps::MapQuads[i].b;
      maps::MapQuads[i].i_right.b = maps::MapQuads[i].c;
      maps::MapQuads[i].i_right.c = maps::MapQuads[i].d;
    }

    if(LOGGING==-1)
    {
      print_out_quads(maps::MapQuads);
    }
  }


  void drop_map()
  {
    /*
      Drops current map

      - needs to remove quads
      - what if next map will be bigger?

      - dont know
    */

    for(int q = 0; q < maps::MapQuads.size(); q++)
    {
      quads::delete_quad_id(maps::MapQuads[q].id);
      quads::delete_vertex_id(maps::MapQuads[q].a);
      quads::delete_vertex_id(maps::MapQuads[q].b);
      quads::delete_vertex_id(maps::MapQuads[q].c);
      quads::delete_vertex_id(maps::MapQuads[q].d);
    }
    maps::MapQuads.clear();
  }
}

#endif