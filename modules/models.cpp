#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "models.h"
#include "quads.h"
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
  phmap::flat_hash_map<int, int> map_sizes;
  phmap::flat_hash_map<int, int> map_type_count;
  std::vector<models::ModelMeshData> meshes;

  void init()
  {
    models::map_sizes[5126] = 4; // float
    models::map_sizes[5123] = 2; // unsigned short
    models::map_sizes[5120] = 1; // byte
    models::map_sizes[5121] = 1; // unsigned byte
    models::map_sizes[5124] = 4; // int
    models::map_sizes[5125] = 4; // unsigned int
    models::map_sizes[5130] = 8; // double

    models::map_type_count[3] = 3;
    models::map_type_count[2] = 2;
    models::map_type_count[65] = 1;
    models::map_type_count[4] = 4;

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

      // models::_print_scenes(model_id);
      // models::_print_nodes(model_id);
      // models::_print_meshes(model_id);
      // models::_print_buffer_views(model_id);
      // models::_print_accessors(model_id);
      // models::_print_buffers(model_id);
      models::extract_meshes(model_id);
    }
  }

  void refresh()
  {
    models::Index.clear();
    models::models.clear();
    models::meshes.clear();
    models::map_type_count.clear();
    models::map_sizes.clear();
  }


  void drop(int model_id)
  {
    if(models::models.count(model_id) > 0)
    {
      models::models.erase(model_id);
      utils::drop_id(models::Index, model_id);
    }

  }


  void _print_scenes(int model_id)
  {
    std::cout << std::endl;
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
    std::cout << std::endl;
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
    std::cout << std::endl;
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
    std::cout << std::endl;
    for(int a = 0; a < models::models[model_id].accessors.size(); a++)
    {
      std::cout << "Accessor: " << a 
      << " bufferView: " << models::models[model_id].accessors[a].bufferView
      << " byteOffset: " << models::models[model_id].accessors[a].byteOffset
      << " componentType: " << models::models[model_id].accessors[a].componentType
      << " count: " << models::models[model_id].accessors[a].count
      << " type: " << models::models[model_id].accessors[a].type
      << std::endl;
    }
  }


  void _print_buffer_views(int model_id)
  {
    std::cout << std::endl;
    for(int b = 0; b < models::models[model_id].bufferViews.size(); b++)
    {
      std::cout << "Buffer View: " << b
      << " buffer: " << models::models[model_id].bufferViews[b].buffer
      << " byteLength: " << models::models[model_id].bufferViews[b].byteLength
      << " byteOffset: " << models::models[model_id].bufferViews[b].byteOffset
      << " target: "  << models::models[model_id].bufferViews[b].target
      << std::endl;
    }
  }


  void _print_buffers(int model_id)
  {
    std::cout << std::endl;
    for(int b = 0; b < models::models[model_id].buffers.size(); b++)
    {
      std::cout << "Buffer: " << b 
      //<< " uri: " << models::models[model_id].buffers[b].data
      << std::endl;

      // for(int v=0; v < models::models[model_id].buffers[b].data.size(); v++)
      // {
      //   std::cout << float(models::models[model_id].buffers[b].data[v]);
      // }
      // std::cout << std::endl;
    }
  }

  std::vector<float> _extract_floats(int count, int element_count, int stride, std::vector<unsigned char>& subdata)
  {
    int offset = 0;
    std::vector<float> mega_vector;
    int v_size = count*element_count;
    mega_vector.reserve(v_size);

    for(int i=0; i < v_size; i++)
    { 
      unsigned char byte_arr[stride];
      for(int b=0; b < stride; b++)
      {
        byte_arr[b] = subdata[offset];
        offset++;
      }
      float value = (*(float*)byte_arr);
      mega_vector.push_back(value);
    }
    return mega_vector;
  }

  // Trying to convert to mesh data I assume
  models::ModelMeshData convert_mesh_data(int model_id, int mesh_id, int node_id) 
  {   
    models::ModelMeshData MMD;
    MMD.model_id = model_id;
    MMD.mesh_id = mesh_id;
    MMD.node_id = node_id;

    // loop through Mesh primitives
    for(int p=0; p < models::models[model_id].meshes[mesh_id].primitives.size(); p++)
    {
      // map of attributes <name, accessor_id> - POSITION, NORMAL, TEXCOORD_0 
      for (auto const& [attrb_name, accessor_id] : models::models[model_id].meshes[mesh_id].primitives[p].attributes)
      {
        // buffer view index
        int bv_id = models::models[model_id].accessors[accessor_id].bufferView;
        int comp_type = models::models[model_id].accessors[accessor_id].componentType;
        int type = models::models[model_id].accessors[accessor_id].type;
        int count = models::models[model_id].accessors[accessor_id].count;

        int buffer_id = models::models[model_id].bufferViews[bv_id].buffer;
        int byte_length = models::models[model_id].bufferViews[bv_id].byteLength;
        int byte_offset = models::models[model_id].bufferViews[bv_id].byteOffset;

        std::vector<unsigned char> subdata = {models::models[model_id].buffers[buffer_id].data.begin() + byte_offset, 
                                              models::models[model_id].buffers[buffer_id].data.begin() + byte_offset + byte_length}; 

        // std::cout << " comp_type : " << comp_type  << std::endl;
        // std::cout << " count : " << count  << std::endl;
        // std::cout << " type : " << type  << std::endl;
        // std::cout << " subdata size :" << subdata.size() << std::endl;
        
        // SUBDATA SIZE = COUNT * TYPE() * FLOAT SIZE(stride) 1560 = 130*3*4

        int stride = models::map_sizes[comp_type];
        int element_count = models::map_type_count[type];
        // std::cout << " stride : " << stride  << std::endl;
        // std::cout << " element_count : " << element_count  << std::endl;

        std::vector<float> mega_vector = models::_extract_floats(count, element_count, stride, subdata);

        // :( I know its a string comparison but cant find any better solution for this right now
        // It doesnt happen often (just on the data reading part)
        if(attrb_name == "POSITION")
        {
          MMD.position = mega_vector;
          MMD.count_vertices = count;
        }

        if(attrb_name == "NORMAL")
        {
          MMD.norms = mega_vector;
        }

        if(attrb_name == "TEXCOORD_0")
        {
          MMD.texcoord = mega_vector;
        }
      }
      // int material_id = models::models[model_id].meshes[mesh_id].primitives[p].material;
      // int indices_acc = models::models[model_id].meshes[mesh_id].primitives[p].indices;
    }

    return MMD;
  } 



  void extract_meshes(int model_id)
  {
    // scene loop (I assume it will always be one scene). Scene contains nodes
    for(int s=0; s < models::models[model_id].scenes.size(); s++)
    {
      // Node loop, node:  mesh, name, rotation, translation, scale, children
      for(int n=0; n < models::models[model_id].scenes[s].nodes.size(); n++)
      {
        int node_id = models::models[model_id].scenes[s].nodes[n];
        int mesh_id = models::models[model_id].nodes[node_id].mesh;
        models::ModelMeshData MMD = convert_mesh_data(model_id, mesh_id, node_id);
        models::meshes.push_back(MMD);
      }
    }
  }

}

