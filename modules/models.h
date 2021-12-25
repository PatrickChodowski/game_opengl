#include <string>
#include <vector>

#include "../dependencies/tiny_gltf.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_MODELS_H
#define MODULES_MODELS_H

namespace models 
{
  extern std::vector<int> Index;
  extern phmap::flat_hash_map<int, tinygltf::Model> models;

  // Read all model files
  void init();

  // Reads in model data
  void read_data(std::string& file_name);

  // Refreshes the data
  void refresh();

  // Drops model by its id
  void drop(int model_id);


}

#endif
