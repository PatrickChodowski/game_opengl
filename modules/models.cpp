#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "fonts.h"
#include "models.h"
#include "quads.h"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
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
  phmap::flat_hash_map<int, int> SceneModels;
  std::vector<int> SceneModelsList;
  phmap::flat_hash_map<int, int> ModelTextureMap;


  void init(std::string model_data_path)
  {
    std::vector<std::string> model_list = utils::list_json_files(model_data_path);
    std::sort(model_list.begin(), model_list.end());
    
    for(int m=0; m < model_list.size(); m++)
    {
      models::read_data(model_list[m]);
    };
    std::cout << "Models Initialized" << std::endl;

    // Need to be sure we are doing this after models.
    // Used Font's model data to create fonts::CharacterAtlas
    fonts::init();
  }

  void read_data(std::string& file_name)
  {
    models::ModelData MD;
    std::string file_path = "./data/models/"+file_name+".json";
    std::cout << " [MODELS] Reading data from " << file_path << std::endl;
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.allow_missing_members = true;
    context.parseTo(MD);
    // propagate MD.frames(map) from frames_list(vector)
    for(int f=0; f < MD.frames_list.size(); f++)
    {
      MD.frames.insert({MD.frames_list[f].frame_id, MD.frames_list[f]});
    }
    // Later will clean it
    // MD.frames_list.clear();


    // propagate TD.anims(map) from anims_list(vector)
    for(int a=0; a < MD.anim_list.size(); a++)
    {
      MD.anims.insert({MD.anim_list[a].anim_id, MD.anim_list[a]});
    }
    // Later will clean it
    // MD.anim_list.clear();

    models::models.insert(std::pair<int, models::ModelData>{MD.id, MD});
    unsigned int opengl_texture_id = models::_load_texture_to_opengl(MD.id, MD.w, MD.h, 4);
    models::models.at(MD.id).opengl_texture_id = opengl_texture_id;
  }

  void refresh()
  {

  }

  void log()
  {
    
  }

  void clear()
  {
    models::SceneModels.clear();
    models::SceneModelsList.clear();
  }

  unsigned int _load_texture_to_opengl(unsigned int model_id, int w, int h, int n_channels)
  {
    std::string model_name = models::models.at(model_id).name;
    std::string texture_path = std::string("./data/models/") + std::to_string(model_id) + "_" + model_name + "___spritesheet.png";
    stbi_set_flip_vertically_on_load(false);
    unsigned char *image_data = stbi_load(texture_path.c_str(), &w, &h, &n_channels, 4); 
    if(image_data == NULL)
    {
      std::cout << "Error while loading texture from " << texture_path << std::endl;
    };

    // We generate texture and assign it to texture/model_id. It doesnt have to be the same, but high chance it will
    unsigned int texture_id;
    glGenTextures(1, &texture_id); 
    glBindTexture(GL_TEXTURE_2D, texture_id); 

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
    return texture_id;
  };





  void make_sampler(int* arr, int sampler_size)
  {
    arr[0] = 0;
    for(auto const& [model_id, texture_unit] : models::ModelTextureMap) 
    {
      unsigned int opengl_texture_id = models::models.at(model_id).opengl_texture_id;
      glActiveTexture(GL_TEXTURE0 + texture_unit);
      glBindTexture(GL_TEXTURE_2D, opengl_texture_id);
      arr[texture_unit] = texture_unit;
    } 

  }



  void load(int model_id)
  {
    // Checks if model is already in the scene, if its not, load it
    const bool model_already_in_use = models::SceneModels.find(model_id) != models::SceneModels.end();
    if(!model_already_in_use){
      int new_sampler_index = models::SceneModels.size()+1;
      models::SceneModels.insert(std::pair<int, int>(model_id, new_sampler_index));
      models::SceneModelsList.push_back(model_id);

      models::ModelTextureMap.clear();
      for(int s=0; s<models::SceneModelsList.size(); s++){
        int model_id = models::SceneModelsList[s];
        models::ModelTextureMap.insert(std::pair<int, int>{model_id, s+1});
      }

      std::cout << " [MODELS] Loaded model ID:" << model_id << " NAME: " << models::models.at(model_id).name << " to the scene" << std::endl;
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

  void drop()
  {
    for(auto const& [model_id, model_data] : models::models) 
    {
      glDeleteTextures(1, &model_data.opengl_texture_id);
    } 
  };

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
    std::cout << " Loaded models:" << std::endl;
    for(auto const& [model_id, sampler_texture_index] : models::SceneModels) 
    {
      std::cout << " " << model_id << ";";
    } 
    std::cout << std::endl;
  }


}
