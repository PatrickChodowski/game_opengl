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
  std::vector<int> BoundTextures = {};
  
  
  // loads single texture into memory
  unsigned int load(unsigned int texture_id)
  {
    stbi_set_flip_vertically_on_load(false);  
    std::string texture_path = "assets/"+textures::Catalog[texture_id].name+".png";
    int n_channels = 4;
    // this reads texture information 
    unsigned char *image_data = stbi_load(texture_path.c_str(),
                                         &textures::Catalog[texture_id].width, 
                                         &textures::Catalog[texture_id].height, &n_channels, 4); 

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
                          textures::Catalog[texture_id].width,
                          textures::Catalog[texture_id].height,  
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

    // add to catalog
    Catalog.insert({TD.id, TD});
    unsigned int texture_id = textures::load(TD.id);
    textures::BoundTextures.push_back(texture_id);
 
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
  // reads also the textures themselves
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