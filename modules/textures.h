#include <string>
#include <map>
#include <vector>
#include "../dependencies/json_struct.h"

#ifndef MODULES_TEXTURES_H
#define MODULES_TEXTURES_H

// Textures - Persistent data. Loaded once on the beginning of the level/scene
namespace textures2
{
  // Frame data - id, position, dimensions and label
  struct Frame
  {
    int frame_id;
    float x, y;
    float w, h; 
    std::string label;

    JS_OBJ(frame_id, x, y, w, h, label);
  };

  // Animation key data - id and delay
  struct AnimationKey
  {
    int id;
    int ms_delay;

    JS_OBJ(id, ms_delay);
  };

  // Animation information
  struct Animation
  {
    std::string label;
    int event_id;
    int length;
    int idle_frame;
    std::vector<AnimationKey> sequence;
    std::vector<int> key_list;
    JS_OBJ(label, event_id, length, sequence, idle_frame);
  };

  // Texture data for the catalog
  struct TextureData
  {

    int id;
    unsigned int opengl_texture_id;

    float w;
    float h;

    std::string type;
    std::string name;

    std::vector<Frame> frames_list;
    std::vector<Animation> anims_list;
    std::map<int, Frame> frames;
    std::map<int, Animation> anims;

    JS_OBJ(id, w, h, type, name, frames_list, anims_list);
  };

  // Persisten table of textures data
  extern std::map<int, textures2::TextureData> textures;

  // Vector of bound textures to opengl
  extern std::vector<unsigned int> BoundTextures;

  // Current Font Texture data
  extern textures2::TextureData FontTD;

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

  // Calculates the normalized X position of frame start
  float _get_normalized_frame_start(int texture_id, int frame_id);

  // Calculates the normalized X position of frame end
  float _get_normalized_frame_end(int texture_id, int frame_id);
}


#endif
