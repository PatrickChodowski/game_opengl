#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "models.h"
#include "quads.h"
#include "utils.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "../dependencies/parallel_hashmap/phmap.h"


namespace models 
{
  phmap::flat_hash_map<int, models::ModelData> models;
  // Maximum 16 models allowed at the same time. Keeps values unique
  phmap::flat_hash_set<int> SceneModels;


  void init()
  {
    // Maximum 16 models allowed at the same time
    models::SceneModels.reserve(16);

    std::vector<std::string> model_list = utils::list_json_files("models");
    for(int m=0; m < model_list.size(); m++)
    {
      models::read_data(model_list[m]);
    };
    std::cout << "Models Initialized" << std::endl;
  }

  void read_data(std::string& file_name)
  {
    models::ModelData MD;
    std::string file_path = "./models/"+file_name+".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.allow_missing_members = true;
    context.parseTo(MD);
    models::models.insert(std::pair<int, models::ModelData>{MD.id, MD});
    unsigned int opengl_texture_id = models::_load_texture_to_opengl(MD.id, MD.w, MD.h, 4);
    models::models.at(MD.id).opengl_texture_id = opengl_texture_id;
  }

  void refresh()
  {

  }

  void drop(int model_id)
  {

  }

  void render()
  {

  }

  void log()
  {
    
  }

  void clear()
  {
    models::models.clear();
    models::SceneModels.clear();
  }

  unsigned int _load_texture_to_opengl(unsigned int model_id, int w, int h, int n_channels)
  {
    std::string model_name = models::models.at(model_id).name;
    std::string texture_path = std::string("models/") + std::to_string(model_id) + "_" + model_name + "___spritesheet.png";
    stbi_set_flip_vertically_on_load(false);
    unsigned char *image_data = stbi_load(texture_path.c_str(), &w, &h, &n_channels, 4); 

    if(image_data == NULL)
    {
      std::cout << "Error while loading texture from " << texture_path << std::endl;
    };

    // We generate texture and assign it to texture/model_id. It doesnt have to be the same, but high chance it will
    glGenTextures(1, &model_id); 
    glBindTexture(GL_TEXTURE_2D, model_id); 
    // std::cout << " OpenGL texture/model_id: " << model_id << std::endl;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Send Image data to GPU here
    glTexImage2D(GL_TEXTURE_2D, //target 
                0, //level, 0 is base image
                GL_RGBA, //internalformat
                w,
                h,  
                0,  // border
                GL_RGBA,  // format
                GL_UNSIGNED_BYTE,  // type
                image_data); // data

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_data);
    return model_id;
  };

  void _activate_texture(int model_id)
  {
    unsigned int opengl_texture_id = models::models.at(model_id).opengl_texture_id;

    // Activate selected texture unit
    glActiveTexture(GL_TEXTURE0 + opengl_texture_id);
    glBindTexture(GL_TEXTURE_2D, opengl_texture_id);
  
  };

  void bind()
  {
    for(auto m : models::SceneModels) 
    {
      models::_activate_texture(m);
    } 
  }

  void load(int model_id)
  {
    // Checks if model is already in the scene, if its not, load it
    const bool model_already_in_use = models::SceneModels.find(model_id) != models::SceneModels.end();
    if(!model_already_in_use){
      models::SceneModels.insert(model_id);
      std::cout << "Loaded model ID:" << model_id << " NAME: " << models::models.at(model_id).name << " to the scene" << std::endl;
    }
  };

  void unload(int model_id)
  {
    const bool model_in_use = models::SceneModels.find(model_id) != models::SceneModels.end();
    if(model_in_use){
      // Delete the model
      models::SceneModels.erase(model_id);
      std::cout << "Unloaded model ID:" << model_id << " NAME: " << models::models.at(model_id).name << " from the scene" << std::endl;
    }
  }


  void print_models_data()
  {
    std::cout << "### PRINT MODELS DATA ###" << std::endl;
    for (auto const& [k, v] : models::models)
    {
      std::cout << "Model ID: " << v.id
                << "  Name: " << v.name
                << "  OpenGl Texture ID: " << v.opengl_texture_id
                << "  Frame count: " << v.frames_list.size()
                << "  Anim count: " << v.anim_list.size()
                << std::endl;

      for(int f = 0; f < v.frames_list.size(); f++)
      {
        std::cout << "  - Frame: " << v.frames_list[f].frame_id << " " << v.frames_list[f].label << std::endl;
      }

      for(int a = 0; a < v.anim_list.size(); a++)
      {
        std::cout << "  - Anim: " << v.anim_list[a].anim_id << " " << v.anim_list[a].label << std::endl;
      }
    }
  }


}
