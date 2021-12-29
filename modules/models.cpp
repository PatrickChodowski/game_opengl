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


  std::vector<float> _extract_floats(int model_id, int accessor_id, std::vector<unsigned char>& subdata)
  {
    int count = models::models[model_id].accessors[accessor_id].count;
    int stride = models::map_sizes[models::models[model_id].accessors[accessor_id].componentType];
    int element_count = models::map_type_count[models::models[model_id].accessors[accessor_id].type];

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

  std::vector<float> _extract_floats_via_accessor(int model_id, int accessor_id)
  {
    std::vector<unsigned char> subdata = _get_subdata(model_id, accessor_id);
    std::vector<float> mega_vector = models::_extract_floats(model_id, accessor_id, subdata);
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

    // loop through Mesh primitives -> Extract position, normals, texcoords, indices, material

    for(int p=0; p < models::models[model_id].meshes[mesh_id].primitives.size(); p++)
    {
      int accessor_id;
      int count;
      std::vector<float> mega_vector;

      // Position -> vec3
      if(models::models[model_id].meshes[mesh_id].primitives[p].attributes.count("POSITION") > 0)
      {
        accessor_id = models::models[model_id].meshes[mesh_id].primitives[p].attributes["POSITION"];  
        mega_vector = models::_extract_floats_via_accessor(model_id, accessor_id);
        MMD.count_vertices = models::models[model_id].accessors[accessor_id].count;
        for(int i=0; i<MMD.count_vertices; i++)
        {
          glm::vec3 v = {mega_vector[3*i], mega_vector[(3*i)+1], mega_vector[(3*i)+2]};
          MMD.position.push_back(v);
        }  
      }

      // Normals -> vec3
      if(models::models[model_id].meshes[mesh_id].primitives[p].attributes.count("NORMAL") > 0)
      {
        accessor_id = models::models[model_id].meshes[mesh_id].primitives[p].attributes["NORMAL"];  
        mega_vector = models::_extract_floats_via_accessor(model_id, accessor_id);
        count = models::models[model_id].accessors[accessor_id].count;
        for(int i=0; i<count; i++)
        {
          glm::vec3 v = {mega_vector[3*i], mega_vector[(3*i)+1], mega_vector[(3*i)+2]};
          MMD.norms.push_back(v);
        }  
      }

      // Texcoords -> vec2
      if(models::models[model_id].meshes[mesh_id].primitives[p].attributes.count("TEXCOORD_0") > 0)
      {
        accessor_id = models::models[model_id].meshes[mesh_id].primitives[p].attributes["TEXCOORD_0"];  
        mega_vector = models::_extract_floats_via_accessor(model_id, accessor_id);
        count = models::models[model_id].accessors[accessor_id].count;
        for(int i=0; i<count; i++)
        {
          glm::vec2 v = {mega_vector[2*i], mega_vector[(2*i)+1]};
          MMD.texcoord.push_back(v);
        }  
      }

      // Indices
      if(models::models[model_id].meshes[mesh_id].primitives[p].indices > -1)
      {
        accessor_id = models::models[model_id].meshes[mesh_id].primitives[p].indices;
        count = models::models[model_id].accessors[accessor_id].count;
        std::vector<unsigned char> indices_subdata = _get_subdata(model_id, accessor_id);
        int stride = models::map_sizes[models::models[model_id].accessors[accessor_id].componentType];
        int element_count = models::map_type_count[models::models[model_id].accessors[accessor_id].type];
        MMD.indices = models::_extract_shorts(count, element_count, stride, indices_subdata);
      }

      // Material
      int material_id = models::models[model_id].meshes[mesh_id].primitives[p].material;
      MMD.color = {};
      for(int c=0; c< models::models[model_id].materials[material_id].pbrMetallicRoughness.baseColorFactor.size(); c++)
      {
        MMD.color.push_back(models::models[model_id].materials[material_id].pbrMetallicRoughness.baseColorFactor[c]);
      }
    }


    // Rotation -> convert to glm::vec
    if(models::models[model_id].nodes[node_id].rotation.size() == 4)
    {
      for(int i=0; i<4; i++)
      {
        MMD.rotation[i] = models::models[model_id].nodes[node_id].rotation[i];
      }
    }

    // Translation -> convert to glm::vec
    if(models::models[model_id].nodes[node_id].translation.size() == 3)
    {
      for(int i=0; i<3; i++)
      {
        MMD.translation[i] = models::models[model_id].nodes[node_id].translation[i];
      }
    }

    // Translation -> convert to glm::vec
    if(models::models[model_id].nodes[node_id].scale.size() == 3)
    {
      for(int i=0; i<3; i++)
      {
        MMD.scale[i] = models::models[model_id].nodes[node_id].scale[i];
      }
    } else {
      MMD.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    // TEST

    // Initialize matrices
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    // Use translation, rotation, and scale to change the initialized matrices
    trans = glm::translate(trans, MMD.translation);
    rot = glm::mat4_cast(MMD.rotation);
    sca = glm::scale(sca, MMD.scale);

    glm::mat4 local_mvp = trans * rot;

    // update positions

    for(int p=0; p<MMD.position.size(); p++)
    {
      MMD.position[p] = local_mvp*glm::vec4(MMD.position[p], 1.0f);
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

      MMVD.x = MMD.position[i].x;
      MMVD.y = MMD.position[i].y;
      MMVD.z = MMD.position[i].z;

      MMVD.r = MMD.color[0];
      MMVD.g = MMD.color[1];
      MMVD.b = MMD.color[2];
      MMVD.a = MMD.color[3];

      MMVD.tx_x = MMD.texcoord[i].x;
      MMVD.tx_y = MMD.texcoord[i].y;
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

        // Position
        log_file << " \"position\": [";
        for(int n = 0; n < models::meshes[i].position.size(); n++)
        {
          log_file << models::meshes[i].position[n].x << "," << models::meshes[i].position[n].y << "," << models::meshes[i].position[n].z << ",\n";
        } 
        log_file << "],\n";

        // Color
        log_file << " \"color\": [";
        for(int n = 0; n < models::meshes[i].color.size(); n++)
        {
          log_file << models::meshes[i].color[n] << ",";
        } 
        log_file << "],\n";

        // indices
        log_file << " \"indices\": [";
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
