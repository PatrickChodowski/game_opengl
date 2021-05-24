#ifndef TEXTURES_H
#define TEXTURES_H


namespace textures
{ 

  bool has_ending (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
  }


  // list all textures that have metadata created, default path
  std::vector<std::string> list_textures(std::string path="assets/data/")
  {
    std::vector<std::string> list_of_textures = {};
    struct dirent *entry;
    DIR *dir = opendir(path.c_str());
    while ((entry = readdir(dir)) != NULL) {
      if(has_ending (entry->d_name, ".json")){
        std::string str = entry->d_name;
        std::string clean_str = str.replace(str.end()-5, str.end(), "");
        logger::print(clean_str);
        list_of_textures.push_back(clean_str);  
      }
    }
    closedir(dir);
    return list_of_textures;
  };  


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

  // Reading in text files (may be moved later to utils if needed)
  std::string read_text_file(std::string path)
  {
    std::ifstream json_file(path);
    std::ostringstream tmp;
    tmp << json_file.rdbuf();
    std::string s = tmp.str();
    return s;
  } 
  
  // Read-in all json files
  void read_texture_data(std::string name)
  {
    TextureData TD;
    std::string data_path = "./assets/data/"+name+".json";
    std::string json_data = read_text_file(data_path);
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
    std::vector<std::string> texture_list = list_textures();
    for(int t=0; t < texture_list.size(); t++)
    {
      read_texture_data(texture_list[t]);
    }
  }

}












#endif