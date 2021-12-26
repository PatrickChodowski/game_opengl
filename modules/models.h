#include <string>
#include <vector>

#include "../dependencies/tiny_gltf.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_MODELS_H
#define MODULES_MODELS_H

namespace models 
{
  // Keeps all the converted meshes data for given models
  struct ModelMeshData
  {
    int model_id;
    int mesh_id;
    int node_id;
    int count_vertices;

    std::vector<float> position;
    std::vector<float> norms;
    std::vector<float> texcoord;



    std::string mesh_name;


  };




  extern std::vector<int> Index;
  extern phmap::flat_hash_map<int, tinygltf::Model> models;
  extern phmap::flat_hash_map<int, int> map_sizes;
  extern phmap::flat_hash_map<int, int> map_type_count;

  // Read all model files
  void init();

  // Reads in model data
  void read_data(std::string& file_name);

  // Refreshes the data
  void refresh();

  // Drops model by its id
  void drop(int model_id);


  void _print_scenes(int model_id);
 
  void _print_nodes(int model_id);

  void _print_meshes(int model_id);

  void _print_accessors(int model_id);

  void _print_buffer_views(int model_id);

  void _print_buffers(int model_id);

  std::vector<float> _extract_floats(int count, int element_count, int stride, std::vector<unsigned char>& subdata);

  void extract_meshes(int model_id);

  models::ModelMeshData convert_mesh_data(int model_id, int mesh_id, int node_id); 


}

#endif
