
#include <chrono>
#include <ctime> 
#include <string>
#include <vector>

#include "textures.h"
#include "timer.h"
#include "utils.h"

#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_ANIMS_H
#define MODULES_ANIMS_H

namespace anims
{
  // Catalog of entity_id, Animation - current animations played
  extern phmap::flat_hash_map<int, textures::Animation> animsplayed;

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
  bool _check_if_entity_in_anim_same_type(int anim_id, int entity_id);

  // Start animation for given entity
  void start(int anim_id, int entity_id);

  // Runs the single animation from animsplayed
  void play(int entity_id);

}

#endif

