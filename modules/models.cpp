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

// https://github.com/KhronosGroup/glTF-Tutorials/tree/master/gltfTutorial

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

      models::_print_scenes(model_id);
      models::_print_nodes(model_id);
      models::_print_meshes(model_id);
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

  // Retrieve info from model
  // scenes 
  // nodes
  // meshes
  // Attributes of the mesh
  // Accessors
  // BufferViews
  // Buffers

  void _print_scenes(int model_id)
  {
    for(int s = 0; s < models::models[model_id].scenes.size(); s++)
    {
      std::cout << "Scene " << s <<  " name: " << models::models[model_id].scenes[s].name << std::endl;

      for(int n=0; n < models::models[model_id].scenes[s].nodes.size(); n++)
      {
        std::cout << "  Node: " << models::models[model_id].scenes[s].nodes[n] << std::endl;
      }
    }
  }

  void _print_nodes(int model_id)
  {
    for(int n = 0; n < models::models[model_id].nodes.size(); n++)
    {
      std::cout << "Node " << n 
      << " mesh: " << models::models[model_id].nodes[n].mesh 
      << " name: " << models::models[model_id].nodes[n].name
      << " translation size: " << models::models[model_id].nodes[n].translation.size() 
      << " scale size: " << models::models[model_id].nodes[n].scale.size() 
      << " rotation size:" << models::models[model_id].nodes[n].rotation.size()
      << " children size:" << models::models[model_id].nodes[n].children.size() << std::endl;
    }

  }

  void _print_meshes(int model_id)
  {
    for(int m = 0; m < models::models[model_id].meshes.size(); m++)
    {
      std::cout << "Mesh " << m
      << " name: " << models::models[model_id].meshes[m].name << std::endl;

      for(int p = 0; p < models::models[model_id].meshes[m].primitives.size(); p++)
      {
        std::cout << "  Primitive " << p 
        << " indices: " << models::models[model_id].meshes[m].primitives[p].indices 
        << " material: " << models::models[model_id].meshes[m].primitives[p].material
        << " attributes: "
        << std::endl;

        for (auto const& [k, v] : models::models[model_id].meshes[m].primitives[p].attributes)
        {
          std::cout << "  -  " << k << ": " << v << std::endl;
        }
      }
    }
  }

//https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_005_BuffersBufferViewsAccessors.md
  void _print_accessors(int model_id)
  {

  }





}

