
#include <chrono>
#include <ctime> 
#include <string>
#include <vector>

#include "timer.h"
#include "utils.h"

#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_ANIMS_H
#define MODULES_ANIMS_H

namespace anims
{
  // All data needed for all animation types
  struct BaseAnimation
  {
    int id;
    int main_anim_type_id;
    int entity_id;
    int current_keyframe_index;
    float time_length;
    float time_elapsed;
    float next_update_time;
    std::vector<float> update_times;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    bool cyclical;
    bool breakable;
  };


  // Data needed for frame animation only and base animation
  struct FrameAnimation : BaseAnimation
  {
    int texture_id;
    std::vector<int> frame_ids;

    JS_OBJ(id, main_anim_type_id, texture_id, 
    time_length, frame_ids, 
    update_times, cyclical, breakable);
  }; 


  // Data needed for color animation only and base animation
  struct ColorAnimation : BaseAnimation
  {
    std::vector<float> r;
    std::vector<float> g;
    std::vector<float> b;
    std::vector<float> a;

    JS_OBJ(id, main_anim_type_id, r, g, b, a, 
    time_length, 
    update_times, cyclical, breakable);
  }; 

  // Catalog of color Animations
  extern phmap::flat_hash_map<int, anims::ColorAnimation> color_anims;

  // Catalog of frame Animations
  extern phmap::flat_hash_map<int, anims::FrameAnimation> frame_anims;

  // Catalog of entity_id, Animation - current animations played
  extern phmap::flat_hash_map<int, anims::FrameAnimation> animsplayed;

  // index of animations to delete
  extern std::vector<int> anims_to_stop;

  // Reads in all the data
  void init();

  // Reads data for selected frame animation
  void _read_frame_anim_data(std::string& name);

  // Reads data for selected color animation
  void _read_color_anim_data(std::string& name);

  // Clear temporary data
  void clear();

  // Updates all current animations played
  void update();

  // Delete animation from current anims
  void drop(int entity_id);

  // Clear permanent data
  void refresh();

  // Checks if given entity already is playing animation
  bool _check_if_entity_in_anim(int entity_id);

  // Checks if given entity has animation and if this is the same type
  bool _check_if_entity_in_anim_same_type(int anim_type_id, int entity_id);

  // Start animation for given entity
  void start(int anim_type_id, int entity_id);

  // Runs the single animation from animsplayed
  void play(int entity_id);

}

#endif

