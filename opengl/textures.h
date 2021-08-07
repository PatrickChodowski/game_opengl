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
    // unsigned int initialized
    stbi_set_flip_vertically_on_load(false);  
    std::string texture_path = "assets/"+name+".png";
    std::cout << "Texture id: " << texture_id << " path: " << texture_path << std::endl;

    // this reads texture information 
    unsigned char *image_data = stbi_load(texture_path.c_str(),
                                         &width, 
                                         &height, &n_channels, 4); 

    if (image_data == NULL){logger::print("Cannot load texture from path "+texture_path);};

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
    logger::print("Opengl texture id: " + std::to_string(texture_id));
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
    textures::Catalog.insert({TD.id, TD});
  }

  void load(int texture_id){
    unsigned int opengl_texture_id = textures::load_to_opengl(textures::Catalog[texture_id].id, 
                                                              textures::Catalog[texture_id].width, 
                                                              textures::Catalog[texture_id].height, 
                                                              4, 
                                                              textures::Catalog[texture_id].name);
    textures::Catalog[texture_id].opengl_texture_id = opengl_texture_id;

    std::cout << "open texture id: " << opengl_texture_id << " original texture id: " 
    << textures::Catalog[texture_id].id << std::endl;

    textures::BoundTextures.push_back(textures::Catalog[texture_id].opengl_texture_id);
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




  // reads-in all possible textures into a catalog
  // reads also the textures themselves
  void init()
  {
    logger::print("READING TEXTURES");
    std::vector<std::string> texture_list = utils::list_files("assets/data/");
    logger::print("TEXTURE LIST:");
    logger::print_vector(texture_list);

    // read texture data by name
    for(int t=0; t<texture_list.size(); t++)
    {
      textures::read_texture_data(texture_list[t]);
    }

    logger::print("Texture catalog size: " + std::to_string(textures::Catalog.size()));

    // load to opengl by texture_id
    for (auto const &x : textures::Catalog)
    {
      std::cout << "Loading texture id: " << x.first << " from catalog to opengl" << std::endl;
      textures::load(x.first);
    }
  }
  // void unbind()
  // {
  //   GlCall(glBindTexture(GL_TEXTURE_2D, 0));
  // };


  void drop()
  {

    for(int t=0; t<textures::BoundTextures.size(); t++)
    {

        GlCall(glDeleteTextures(1, &textures::BoundTextures[t]));
    } 
    textures::BoundTextures.clear();
  }

}












#endif