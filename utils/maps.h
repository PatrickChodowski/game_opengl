#ifndef MAPS_H
#define MAPS_H


namespace maps 
{

  struct Door
  {
    int door_id;
    int x;
    int y;
    int dest_map_id;
    int player_enter_x;
    int player_enter_y;

    JS_OBJ(door_id, x, y, dest_map_id, player_enter_x, player_enter_y);
  };

  struct MapData
  {
    int id;
    std::string name;
    int vertex_width;
    int vertex_height;
    int texture_id;
    int default_player_x;
    int default_player_y;
    std::vector<Door> doors;

    JS_OBJ(id, name, vertex_width, vertex_height, texture_id, default_player_x, default_player_y, doors);
  };

  std::map<int, MapData> Catalog = {};
  void read_map_data(std::string name)
  {
    
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

  // reads-in all possible textures into a catalog
  void init()
  { 
    logger::print("READING MAPS");
    std::vector<std::string> maps_list = utils::list_files("maps/data/");
    for(int m=0; m < maps_list.size(); m++)
    {
      read_map_data(maps_list[m]);
    }
  }
}

#endif