
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
  struct Animation
  {
    int id;
    int entity_id;
    int texture_id;
    int next_frame_id;
    int current_frame_index;

    std::vector<int> frame_ids;
    std::vector<float> frame_times;
    float next_e_time;
    float time_length;
    float time_elapsed;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    bool cyclical;
    bool breakable;

    JS_OBJ(id, texture_id, next_frame_id, time_length, frame_ids, 
    frame_times, cyclical, breakable);

  };

  // Catalog of animation_id, Animation data - all animations in one place
  extern phmap::flat_hash_map<int, anims::Animation> anims;

  // Catalog of entity_id, Animation - current animations played
  extern phmap::flat_hash_map<int, anims::Animation> animsplayed;

  // index of animations to delete
  extern std::vector<int> anims_to_stop;

  // Reads in all the data
  void init();

  // Reads data for selected animation
  void read_data(std::string& name);

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

