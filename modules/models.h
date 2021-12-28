#include <string>
#include <vector>

#include "../dictionary.h"
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
    std::vector<float> indices;
    std::vector<float> color;

    std::string mesh_name;
  };


  struct ModelMeshVertexData
  {
    int model_vertex_id;
    int model_id;
    int mesh_id;

    float x,y,z;
    float r,g,b,a;
    float tx_x, tx_y;

    // what to do
    float frame_id = 0;
    float texture_id = 0;
    float is_clicked = 0;
    float object_type = OBJECT_TYPE_MODEL;
    float camera_type = CAMERA_DYNAMIC;

  };

  extern std::vector<int> Index;
  extern phmap::flat_hash_map<int, tinygltf::Model> models;
  extern phmap::flat_hash_map<int, int> map_sizes;
  extern phmap::flat_hash_map<int, int> map_type_count;
  extern std::vector<models::ModelMeshData> meshes;
  extern std::vector<models::ModelMeshVertexData> MeshVertices;

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

  float _convert_bytes_to_float(unsigned char* byte_arr, int size, bool to_print = false);

  void _convert_float_to_bytes(float value);

  std::vector<float> _extract_via_accessor(int model_id, int accessor_id);

  std::vector<float> _extract_floats(int count, int element_count, int stride, std::vector<unsigned char>& subdata);

  models::ModelMeshData convert_mesh_data(int model_id, int mesh_id, int node_id); 

  void extract_meshes(int model_id);

  void make_mesh_vertex(models::ModelMeshData& MMD);

  void render();


}

#endif
