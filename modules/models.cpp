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
  std::vector<models::ModelMeshVertexData> MeshVertices;

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

  float _convert_bytes_to_float(unsigned char* byte_arr, int size, bool to_print)
  {
    float value = (*(float*)byte_arr);
    if(to_print)
    {
      std::printf("Bytes: 0x%02X 0x%02X 0x%02X 0x%02X Value: %f \n", byte_arr[0] , byte_arr[1] , byte_arr[2] , byte_arr[3], value);
    }
    return value;
  }

  unsigned short _convert_bytes_to_short(unsigned char* byte_arr, int size, bool to_print)
  {
    unsigned short value = (*(unsigned short*)byte_arr);
    if(to_print)
    {
      std::printf("Bytes: 0x%02X 0x%02X 0x%02X 0x%02X Value: %d \n", byte_arr[0] , byte_arr[1] , byte_arr[2] , byte_arr[3], value);
    }
    return value;
  }

  void _convert_float_to_bytes(float value)
  {
    unsigned char const * p = reinterpret_cast<unsigned char const *>(&value);
    for (std::size_t i = 0; i != sizeof(float); ++i)
    {
        std::printf("The byte #%zu is 0x%02X\n", i, p[i]);
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
      float value = models::_convert_bytes_to_float(byte_arr, stride);
      mega_vector.push_back(value);
    }
    return mega_vector;
  }


  std::vector<unsigned short> _extract_shorts(int count, int element_count, int stride, std::vector<unsigned char>& subdata)
  {
    int offset = 0;
    std::vector<unsigned short> mega_vector;
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
      unsigned short value = models::_convert_bytes_to_short(byte_arr, stride);
      mega_vector.push_back(value);
    }
    return mega_vector;
  }

  std::vector<unsigned char> _get_subdata(int model_id, int accessor_id)
  {
    // buffer view index
    int bv_id = models::models[model_id].accessors[accessor_id].bufferView;
    int count = models::models[model_id].accessors[accessor_id].count;

    int buffer_id = models::models[model_id].bufferViews[bv_id].buffer;
    int byte_length = models::models[model_id].bufferViews[bv_id].byteLength;
    int byte_offset = models::models[model_id].bufferViews[bv_id].byteOffset;

    std::vector<unsigned char> subdata = {models::models[model_id].buffers[buffer_id].data.begin() + byte_offset, 
                                          models::models[model_id].buffers[buffer_id].data.begin() + byte_offset + byte_length}; 

    return subdata;
  }


  std::vector<float> _extract_via_accessor(int model_id, int accessor_id)
  {
    std::vector<unsigned char> subdata = _get_subdata(model_id, accessor_id);
    // SUBDATA SIZE = COUNT * TYPE() * FLOAT SIZE(stride) 1560 = 130*3*4
    int count = models::models[model_id].accessors[accessor_id].count;
    int stride = models::map_sizes[models::models[model_id].accessors[accessor_id].componentType];
    int element_count = models::map_type_count[models::models[model_id].accessors[accessor_id].type];
    std::vector<float> mega_vector = models::_extract_floats(count, element_count, stride, subdata);
    return mega_vector;
  }




  // Trying to convert to mesh data I assume
  models::ModelMeshData convert_mesh_data(int model_id, int mesh_id, int node_id) 
  {   
    models::ModelMeshData MMD;
    MMD.model_id = model_id;
    MMD.mesh_id = mesh_id;
    MMD.node_id = node_id;
    MMD.mesh_name = models::models[model_id].meshes[mesh_id].name;

    // loop through Mesh primitives
    for(int p=0; p < models::models[model_id].meshes[mesh_id].primitives.size(); p++)
    {
      // map of attributes <name, accessor_id> - POSITION, NORMAL, TEXCOORD_0 
      for (auto const& [attrb_name, accessor_id] : models::models[model_id].meshes[mesh_id].primitives[p].attributes)
      {
        std::vector<float> mega_vector = models::_extract_via_accessor(model_id, accessor_id);
        // :( I know its a string comparison but cant find any better solution for this right now
        // It doesnt happen often (just on the data reading part)
        if(attrb_name == "POSITION"){
          MMD.position = mega_vector;
          MMD.count_vertices = models::models[model_id].accessors[accessor_id].count;
        } else if(attrb_name == "NORMAL"){
          MMD.norms = mega_vector;
        } else if(attrb_name == "TEXCOORD_0"){
          MMD.texcoord = mega_vector;
        }
      }

      // Indices
      int indices_accessor = models::models[model_id].meshes[mesh_id].primitives[p].indices;
      int indices_count = models::models[model_id].accessors[indices_accessor].count;
      std::vector<unsigned char> indices_subdata = _get_subdata(model_id, indices_accessor);
      int stride = models::map_sizes[models::models[model_id].accessors[indices_accessor].componentType];
      int element_count = models::map_type_count[models::models[model_id].accessors[indices_accessor].type];
      MMD.indices = models::_extract_shorts(indices_count, element_count, stride, indices_subdata);

      // Material
      int material_id = models::models[model_id].meshes[mesh_id].primitives[p].material;
      MMD.color = {};
      for(int c=0; c< models::models[model_id].materials[material_id].pbrMetallicRoughness.baseColorFactor.size(); c++)
      {
        MMD.color.push_back(models::models[model_id].materials[material_id].pbrMetallicRoughness.baseColorFactor[c]);
      }
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


  void make_mesh_vertex(models::ModelMeshData& MMD)
  {
    for(int i=0; i<MMD.count_vertices; i++)
    {
      models::ModelMeshVertexData MMVD;
      MMVD.model_id = MMD.model_id;
      MMVD.mesh_id = MMD.mesh_id;
      MMVD.model_vertex_id = i;

      MMVD.x = MMD.position[3*i];
      MMVD.y = MMD.position[(3*i)+1];
      MMVD.z = MMD.position[(3*i)+2];

      MMVD.r = MMD.color[4*i];
      MMVD.g = MMD.color[(4*i)+1];
      MMVD.b = MMD.color[(4*i)+2];
      MMVD.a = MMD.color[(4*i)+3];

      MMVD.tx_x = MMD.texcoord[2*i];
      MMVD.tx_y = MMD.texcoord[(2*i)+1];
      models::MeshVertices.push_back(MMVD);
    }
  }

  void render()
  {
    models::MeshVertices.clear();
    for(int m=0; m<models::meshes.size(); m++)
    {
      models::make_mesh_vertex(models::meshes[m]);
    }

    //std::cout << "mesh vertices size: " << models::MeshVertices.size() << std::endl;

  }


  void log()
  {
    // Log mesh data
    const char* log_path = "logs/models.json";
    std::ofstream log_file (log_path);
    std::string end_str = " }, \n";
    if (log_file.is_open())
    {
      log_file << "[ \n";
      for(int i = 0; i < models::meshes.size(); i++)
      {
        if(i == (models::meshes.size() - 1))
        {
          end_str = " } \n";
        }
        log_file << " { \n" <<
                    " \"model_id\": " << models::meshes[i].model_id << ",\n"
                    " \"mesh_id\": " << models::meshes[i].mesh_id << ",\n"
                    " \"node_id\": " << models::meshes[i].node_id << ",\n"
                    " \"count_vertices\": " << models::meshes[i].count_vertices << ",\n"
                    " \"mesh_name\": \"" << models::meshes[i].mesh_name << "\",\n";

        log_file <<        " \"indices\": [";
        for(int n = 0; n < models::meshes[i].indices.size(); n++)
        {
          log_file << models::meshes[i].indices[n] << ",";
        } 
        log_file << "]\n";
        log_file << end_str;
      }
      log_file << "] \n";
      log_file.close();
    }
  }
}
