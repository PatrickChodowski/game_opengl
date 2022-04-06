#include <string>
#include <vector>

#include "../dictionary.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/parallel_hashmap/btree.h"

#ifndef MODULES_MODELS_H
#define MODULES_MODELS_H

namespace models 
{
  // Models animation data
  struct ModelAnimData
  {
    bool breakable;

    int anim_id;
    int frame_count;
    int next_anim_id = ANIM_STANDING_IDLE;
    int anim_type_id = ANIM_TYPE_FRAME; // currently default

    // Provided while initializing the animation
    int entity_id = -1;
    int CK_ID = -1;
    int id; // Unique animation id?

    std::vector<int> frame_id;
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
    float time_elapsed; // time elapsed in miliseconds
    float next_update_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    std::string label;

    JS_OBJ(breakable, anim_id, frame_count, next_anim_id,
    frame_id, r, g, b, a, w, h, x, y, z, update_times, time_length, 
    label);

  };

  // Data for model's single frame. Frame ID is built as  animation_id*10000 + side_id*100 + frame_index
  struct ModelFrameData
  {
    int frame_id;
    int x;
    int y;
    int w;
    int h;

    float norm_x_start;
    float norm_x_end;
    float norm_y_start;
    float norm_y_end;

    int hook_x;
    int hook_y;
    int left_hand_x;
    int left_hand_y;
    int right_hand_x;
    int right_hand_y;
    int head_x;
    int head_y;
    std::string label;

    bool is_solid;

    JS_OBJ(frame_id, x, y, w, h, 
           norm_x_start, norm_x_end, norm_y_start, norm_y_end,
           hook_x, hook_y,
           left_hand_x, left_hand_y, 
           right_hand_x, right_hand_y, 
           head_x, head_y, 
           label, is_solid);
  };

  // Model data read in from the file
  struct ModelData
  {
    int id;
    int w;
    int h;
    unsigned int opengl_texture_id;
    std::vector<models::ModelFrameData> frames_list;
    std::vector<models::ModelAnimData> anim_list;

    // Propagated after reading
    phmap::btree_map<int, models::ModelFrameData> frames;
    phmap::btree_map<int, models::ModelAnimData> anims;

    std::string name;
    JS_OBJ(id, w, h, name, frames_list, anim_list);
  };
  extern phmap::flat_hash_map<int, models::ModelData> models;

  // List of models used in the given scene. Contains Model_id and sampler_texture_index
  extern phmap::flat_hash_map<int, int> SceneModels;

  // Read all model files
  void init();

  // Reads in model data
  void read_data(std::string& file_name);

  // Refreshes the data
  void refresh();

  // Drops models from OpenGL [gldeletetexture]
  void drop();

  // Log model data if needed
  void log();

  // Remove all models data
  void clear();

  // Loads texture to opengl
  unsigned int _load_texture_to_opengl(unsigned int model_id, int w, int h, int n_channels);

  // Activate selected texture before render
  void _activate_texture(int model_id);

  // Bind all scene models
  void bind();

  // Loads model by model id (triggered by Scene initialization, map loading, spawning etc.)
  void load(int model_id);

  // Unload model from scene models by model_id
  void unload(int model_id);

  // Populate sampler
  void populate_sampler(int* arr);

  // Prints out models data
  void print_models_data();

}

#endif
