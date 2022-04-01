#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "models.h"
#include "quads.h"
#include "utils.h"

// #define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../dependencies/parallel_hashmap/phmap.h"


namespace models 
{
  std::vector<int> Index;
  phmap::flat_hash_map<int, models::ModelData> models;

  void init()
  {

  }

  void read_data(std::string& file_name)
  {
    ModelData MD;
    std::string file_path = "./models/"+file_name+".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    return MD;
  }

  void refresh()
  {

  }

  void drop(int model_id)
  {

  }

  void render()
  {

  }

  void log()
  {
    
  }


}
