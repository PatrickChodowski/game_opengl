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

  // Animation data read in from the asset/texture file
  struct Animation
  {
    int id;
    int anim_type_id;
    int next_anim_id;

    // Provided while initializing the animation
    int entity_id = -1;
    int CK_ID = -1;

    // frames
    std::vector<int> frame_id;
    std::vector<int> direction;
    // color
    std::vector<float> r;
    std::vector<float> g;
    std::vector<float> b;
    std::vector<float> a;
    // dimension
    std::vector<float> w;
    std::vector<float> h;
    // position
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;

    std::vector<float> update_times;

    float time_length;
    float time_elapsed;
    float next_update_time;
    bool breakable;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    JS_OBJ(id, anim_type_id, 
    frame_id, direction, r, g, b, a, w, h, x, y, z,
    update_times, time_length,
    next_anim_id, breakable);
  };

  // Frame data - id, position, dimensions and label
  struct Frame
  {
    int frame_id;
    float x, y;
    float w, h; 
    float hand_x, hand_y;
    float hook_x, hook_y;

    JS_OBJ(frame_id, x, y, w, h, hand_x, hand_y, hook_x, hook_y);
  };

  // Texture data for the catalog
  struct TextureData
  {

    int id;
    unsigned int opengl_texture_id;

    float w, h;

    std::string type;
    std::string name;

    std::vector<textures::Frame> frames_list;
    std::vector<textures::Animation> anim_list;

    // Propagated after read:
    phmap::btree_map<int, textures::Frame> frames;
    phmap::btree_map<int, textures::Animation> anims;

    JS_OBJ(id, w, h, type, name, frames_list, anim_list);
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
