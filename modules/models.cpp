#include <iostream>
#include <string>
#include <vector>

#include "models.h"
#include "utils.h"


// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "../dependencies/tiny_gltf.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace models 
{

  std::vector<int> Index;
  phmap::flat_hash_map<int, tinygltf::Model> models;

  void init()
  {
    std::vector<std::string> model_list = utils::list_gltf_files("data/models");
    std::string filename;
    for(int n=0; n < model_list.size(); n++)
    {
      std::cout << "reading data for model " << model_list[n] << std::endl;
      filename = "./data/models/"+model_list[n]+".gltf";
      models::read_data(filename);
    };
  }

  void read_data(std::string& file_name)
  {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    int model_id = -1;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, file_name);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

    if (!warn.empty()) 
    {
      printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) 
    {
      printf("Err: %s\n", err.c_str());
    }

    if (!ret) 
    {
      printf("Failed to parse glTF\n");
    } else {

      model_id = utils::generate_id(models::Index);
      models::models[model_id] = model;
    }
  }

  void refresh()
  {
    models::Index.clear();
    models::models.clear();
  }


  void drop(int model_id)
  {
    if(models::models.count(model_id) > 0)
    {
      models::models.erase(model_id);
      utils::drop_id(models::Index, model_id);
    }

  }


}

