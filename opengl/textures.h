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
    std::vector<Frame> frames_list;
    std::map<int, Frame> frames;

    JS_OBJ(id, type, name, width, height, frames_list);
  };

  // Creating catalog of all textures data 
  std::map<int, TextureData> Catalog = {};
  std::vector<int> BoundTextures = {};
  
  
  // loads single texture into memory
  unsigned int load(unsigned int texture_id, int width, int height, int n_channels, std::string name)
  {
    stbi_set_flip_vertically_on_load(false);  
    std::string texture_path = "assets/"+name+".png";
    std::cout << "Texture id: " << texture_id << " path: " << texture_path << std::endl;

    // this reads texture information 
    unsigned char *image_data = stbi_load(texture_path.c_str(),
                                         &width, 
                                         &height, &n_channels, 4); 

    if (image_data == NULL){logger::print("Cannot load texture from path "+texture_path);};



    // generate texture names (number of textures, array in which the generated texture will be stored)
    GlCall(glGenTextures(1, &texture_id)); 
    GlCall(glBindTexture(GL_TEXTURE_2D, texture_id));

    // set those parameters everytime
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));	
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // specify 2 dimmensional texture image
    GlCall((glTexImage2D(GL_TEXTURE_2D, //target 
                          0, //level, 0 is base image
                          GL_RGBA, //internalformat
                          width,
                          height,  
                          0,  // border
                          GL_RGBA,  // format
                          GL_UNSIGNED_BYTE,  // type
                          image_data))); // data

    //  bind a named texture to a texturing target
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));
    stbi_image_free(image_data);

    return texture_id;

  }






  // Read-in all json files
  void read_texture_data(std::string name)
  {
    TextureData TD;
    std::string data_path = "./assets/data/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(TD);

    // propagate TD.frames(map) from frames_list(vector)
    for(int f=0; f < TD.frames_list.size(); f++)
    {
      TD.frames.insert({TD.frames_list[f].frame_id, TD.frames_list[f]}); 
    }

    // add to catalog
    Catalog.insert({TD.id, TD});
    unsigned int texture_id = textures::load(TD.id, TD.width, TD.height, 4, TD.name);
    textures::BoundTextures.push_back(texture_id);
 
    if(LOGGING == 0)
    {
      std::cout << "Read-in texture id: " << TD.id << ", type: " << TD.type << ", name: " <<
       TD.name << ", width: " << TD.width << ", height: " << TD.height << ",  frames count: " << TD.frames.size() << std::endl;

      for(int f=0; f<TD.frames_list.size(); f++)
      {
       std::cout <<  "frame_id: " << TD.frames_list[f].frame_id << ", x: " 
       <<  TD.frames_list[f].x << ", y: "<< TD.frames_list[f].y << ", w: " 
       << TD.frames_list[f].w << ", h: "<< TD.frames_list[f].h << ", is_solid: " << TD.frames_list[f].is_solid << std::endl;
      }
    }
    TD.frames_list.clear();
  }


  void bind(unsigned int texture_id, unsigned int slot)
  {
    GlCall(glActiveTexture(GL_TEXTURE0 + slot));
    GlCall(glBindTexture(GL_TEXTURE_2D, texture_id));
  };





  // reads-in all possible textures into a catalog
  // reads also the textures themselves
  void init()
  {
    logger::print("READING TEXTURES");
    std::vector<std::string> texture_list = utils::list_files("assets/data/");
    for(int t=0; t < texture_list.size(); t++)
    {
      logger::print("reading texture named: " + texture_list[t]);
      read_texture_data(texture_list[t]);
    }
  }

  /// atuomate 

  void drop(unsigned int texture_id)
  {
    GlCall(glDeleteTextures(1, &texture_id));
  };

  void unbind()
  {
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));
  };

}












#endif