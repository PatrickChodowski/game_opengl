#include <string>
#include <vector>


#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/quaternion.hpp"
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

    std::vector<float> color;
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> norms;
    std::vector<glm::vec2> texcoord;

    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::mat4 matrix;

    std::vector<unsigned short> indices;
    std::string mesh_name;
  };

  // to update position and size of the visible object
  struct GameModelMeshData
  {

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

  float _convert_bytes_to_float(unsigned char* byte_arr, int size, bool to_print = false);

  unsigned short _convert_bytes_to_short(unsigned char* byte_arr, int size, bool to_print = false);

  void _convert_float_to_bytes(float value);

  std::vector<float> _extract_floats_via_accessor(int model_id, int accessor_id);

  std::vector<float> _extract_floats(int model_id, int accessor_id, std::vector<unsigned char>& subdata);

  std::vector<unsigned short> _extract_shorts(int count, int element_count, int stride, std::vector<unsigned char>& subdata);

  models::ModelMeshData convert_mesh_data(int model_id, int mesh_id, int node_id); 

  void extract_meshes(int model_id);

  void make_mesh_vertex(models::ModelMeshData& MMD);

  void render();

  void log();

  void log_vertices();

  std::vector<unsigned char> _get_subdata(int model_id, int accessor_id);

}

#endif
