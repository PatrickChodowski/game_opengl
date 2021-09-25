#ifndef TEXTURES_H
#define TEXTURES_H


namespace textures
{   
  // loads single texture into memory
  unsigned int load_to_opengl(unsigned int texture_id, 
                                       int width, 
                                       int height, 
                                       int n_channels, 
                                       std::string name)
  {

    std::string texture_path = "assets/"+name+".png";

    logger::log(LOG_INFO, LOG_EVENT_LOAD_QUADS, "textures::load_to_opengl", 
    __FILE__, __LINE__, "Loading texture id: " + std::to_string(texture_id) + " path: " + texture_path + " to opengl");
    // unsigned int initialized
    stbi_set_flip_vertically_on_load(false);  
    // this reads texture information 
    unsigned char *image_data = stbi_load(texture_path.c_str(),
                                         &width, 
                                         &height, &n_channels, 4); 

    if (image_data == NULL){
          logger::log(LOG_ERROR, LOG_EVENT_LOAD_QUADS, "textures::load_to_opengl", 
    __FILE__, __LINE__, "Texture id: " + std::to_string(texture_id) + " path: " + texture_path + " not loaded to opengl");
    };

    // generate texture names (number of textures, array in which the generated texture will be stored)
    // this changes texture_id to different ID!!!! (in the order of adding )
    GlCall(glGenTextures(1, &texture_id)); 
    GlCall(glBindTexture(GL_TEXTURE_2D, texture_id)); // bind currently generated texture
    // gets new texture_id here

    // set those parameters everytime
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));	
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Send Image data to GPU here
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
    // glBindTexture(target, texture)

    // This call is for unbindining the texture!!!! Hence 0
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));
    stbi_image_free(image_data);

    // returns new texture_id
    logger::log(LOG_INFO, LOG_EVENT_LOAD_QUADS, "textures::load_to_opengl", __FILE__, __LINE__, 
    "Loaded texture new id: " + std::to_string(texture_id));

    return texture_id;
  }

  // Read-in all json files
  void read_texture_data(std::string name)
  {
    std::string data_path = "./assets/data/"+name+".json";
    logger::log(LOG_INFO, LOG_EVENT_READ_DATA, "textures::read_texture_data", __FILE__, __LINE__, 
    "Reading texture data from " + data_path);
    TextureData TD;
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(TD);

    // list animations available for texture (event_id_list)
    for(int a = 0; a < TD.anims_list.size(); a++)
    {
      TD.anims.insert({TD.anims_list[a].event_id, TD.anims_list[a]});
    }
    TD.anims_list.clear();

    // propagate TD.frames(map) from frames_list(vector)
    for(int f=0; f < TD.frames_list.size(); f++)
    {
      TD.frames.insert({TD.frames_list[f].frame_id, TD.frames_list[f]}); 
    }
    TD.frames_list.clear();
    textures::Catalog.insert({TD.id, TD});
  }

  void load(int texture_id)
  {
    logger::log(LOG_INFO, LOG_EVENT_LOAD_QUADS, "textures::load",
     __FILE__, __LINE__, "Loading texture id:  "+std::to_string(texture_id));

    unsigned int opengl_texture_id = textures::load_to_opengl(textures::Catalog[texture_id].id, 
                                                              textures::Catalog[texture_id].width, 
                                                              textures::Catalog[texture_id].height, 
                                                              4, 
                                                              textures::Catalog[texture_id].name);
    textures::Catalog[texture_id].opengl_texture_id = opengl_texture_id;
    textures::BoundTextures.push_back(textures::Catalog[texture_id].opengl_texture_id);

    logger::log(LOG_INFO, LOG_EVENT_LOAD_QUADS, "textures::load",
     __FILE__, __LINE__, "Loaded texture id:  " + std::to_string(texture_id) + 
     "opengl texture id: " + std::to_string(opengl_texture_id));
  }



  void bind_all()
  {
    for(int t=0; t<textures::BoundTextures.size(); t++)
    {
      // which texture slot we are actually binding 
      // first slot -> GL_TEXTURE0
      // Max 32, but depends on platform

      //  Select active texture

      // logger::print("Binding texture :" + std::to_string(textures::BoundTextures[t]));

      GlCall(glActiveTexture(GL_TEXTURE0 + textures::BoundTextures[t]));

      // bind the active texture
      GlCall(glBindTexture(GL_TEXTURE_2D, textures::BoundTextures[t]));
    } 
  }





  // Reads all available texture data and loads textures to opengl
  void init()
  {
    logger::log(LOG_INFO, LOG_EVENT_INIT_MODULE, "textures::init", __FILE__, __LINE__, "Initializing textures");
    std::vector<std::string> texture_list = utils::list_files("assets/data/");

    // read texture data by name
    for(int t=0; t<texture_list.size(); t++)
    {
      textures::read_texture_data(texture_list[t]);
    }
    // load to opengl by texture_id
    for (auto const &x : textures::Catalog)
    {
      textures::load(x.first);
    }
    logger::log(LOG_INFO, LOG_EVENT_INIT_MODULE, "textures::init", __FILE__, __LINE__, "Textures initialized");
  }
  // void unbind()
  // {
  //   GlCall(glBindTexture(GL_TEXTURE_2D, 0));
  // };


  void drop()
  {
    logger::log(LOG_INFO, LOG_EVENT_DROP_DATA, "textures::drop", __FILE__, __LINE__, "Dropping textures");
    for(int t=0; t<textures::BoundTextures.size(); t++)
    {

        GlCall(glDeleteTextures(1, &textures::BoundTextures[t]));
    } 
    textures::BoundTextures.clear();
  }

}












#endif