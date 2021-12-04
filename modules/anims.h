
#include <chrono>
#include <ctime> 
#include <map>
#include <string>
#include <vector>

#include "timer.h"
#include "utils.h"

#include "../dependencies/json_struct.h"

#ifndef MODULES_ANIMS_H
#define MODULES_ANIMS_H

namespace anims
{
  struct KeyFrame
  {
    int frame_id;
    float elapsed_time;

    JS_OBJ(frame_id, elapsed_time);
  };

  struct Animation
  {
    int id;
    int entity_id;
    int texture_id;
    float time_length;
    float time_elapsed;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::vector<anims::KeyFrame> frames;

    bool cyclical;
    bool breakable;

    JS_OBJ(id, texture_id, time_length, frames, cyclical, breakable);

  };

  // Catalog of animation_id, Animation data - all animations in one place
  extern std::map<int, anims::Animation> anims;

  // Catalog of entity_id, Animation - current animations played
  extern std::map<int, anims::Animation> animsplayed;

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
  bool check_if_entity_in_anim(int entity_id);

}

#endif

