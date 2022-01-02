#include <string>
#include <vector>
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/parallel_hashmap/btree.h"

#ifndef MODULES_TEXTURES_H
#define MODULES_TEXTURES_H

// Textures - Persistent data. Loaded once on the beginning of the level/scene
namespace textures
{
  // Frame data - id, position, dimensions and label
  struct Frame
  {
    int frame_id;
    float x, y;
    float w, h; 
    float hand_x, hand_y;
    float hook_x, hook_y;
    std::string label;

    JS_OBJ(frame_id, x, y, w, h, hand_x, hand_y, hook_x, hook_y, label);
  };

  // Texture data for the catalog
  struct TextureData
  {

    int id;
    unsigned int opengl_texture_id;

    float w, h;

    std::string type;
    std::string name;

    std::vector<Frame> frames_list;
    phmap::btree_map<int, textures::Frame> frames;

    JS_OBJ(id, w, h, type, name, frames_list);
  };

  // Persisten table of textures data
  extern phmap::btree_map<int, textures::TextureData> textures;

  // Vector of bound textures to opengl
  extern std::vector<unsigned int> BoundTextures;

  // Hidden method, used to load selected texture to opengl
  unsigned int _load_texture_to_opengl(unsigned int texture_id, int w, int h, int n_channels, std::string name);

  // Read texture data from the path and insert it to textures table
  void read_data(std::string name);

  // Loads texture to opengl and textures catalog
  void load(int texture_id);

  // Binds textures to opengl
  void bind();

  // Reads all available textures and loads them to opengl
  void init();

  // Erases all texture data
  void drop();

  // Logs textures data to json file in logs
  void log();

  // Calculates the normalized X position of frame start
  float _get_normalized_frame_x_start(int texture_id, int frame_id);

  // Calculates the normalized X position of frame end
  float _get_normalized_frame_x_end(int texture_id, int frame_id);

  // Calculates the normalized Y position of frame start
  float _get_normalized_frame_y_start(int texture_id, int frame_id);

  // Calculates the normalized Y position of frame end
  float _get_normalized_frame_y_end(int texture_id, int frame_id);
}


#endif
