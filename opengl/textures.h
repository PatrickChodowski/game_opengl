#ifndef TEXTURES_H
#define TEXTURES_H


namespace textures
{ 

  // Single frame information
  struct Frame
  {
    int frame_id;
    int x;
    int y;
    int w;
    int h;
    int is_solid;

    JS_OBJ(frame_id, x, y, w, h, is_solid);
  };

  // General texture information
  struct TextureData
  {
    int id;
    std::string type;
    std::string name;
    int width;
    int height;
    std::vector<Frame> frames;

    JS_OBJ(id, type, name, width, height, frames);
  };

  // Creating catalog of all textures data 
  std::map<int, TextureData> Catalog = {};
  
  // Read-in all json files
  void read_texture_data(std::string name)
  {
    TextureData TD;
    std::string data_path = "./assets/data/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(TD);

    // add to catalog
    Catalog.insert({TD.id, TD});

    if(LOGGING == 0)
    {
      std::cout << "Read-in texture id: " << TD.id << ", type: " << TD.type << ", name: " <<
       TD.name << ", width: " << TD.width << ", height: " << TD.height << ",  frames count: " << TD.frames.size() << std::endl;

      for(int f=0; f<TD.frames.size(); f++)
      {
       std::cout <<  "frame_id: " << TD.frames[f].frame_id << ", x: " 
       <<  TD.frames[f].x << ", y: "<< TD.frames[f].y << ", w: " 
       << TD.frames[f].w << ", h: "<< TD.frames[f].h << ", is_solid: " << TD.frames[f].is_solid << std::endl;
      }
    }

  }


  // reads-in all possible textures into a catalog
  void init()
  {
    logger::print("READING TEXTURES");
    std::vector<std::string> texture_list = utils::list_files("assets/data/");
    for(int t=0; t < texture_list.size(); t++)
    {
      read_texture_data(texture_list[t]);
    }
  }

}












#endif