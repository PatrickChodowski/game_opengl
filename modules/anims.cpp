
#include <iostream>

#include "anims.h"
#include "entity.h"
#include "utils.h"
#include "textures.h"
#include "timer.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace anims
{
  phmap::flat_hash_map<int, textures::Animation> animsplayed;
  std::vector<int> anims_to_stop;


  void clear()
  {
    anims::animsplayed.clear();
  }

  void drop(int entity_id)
  {
    anims::animsplayed.erase(entity_id);
  }


  void start(int anim_id, int entity_id)
  {
    // Check if given entity's texture has this animation id available in the first place
    if(textures::textures[entity::entities[entity_id].texture_id].anims.count(anim_id) > 0)
    {
      if((!_check_if_entity_in_anim(entity_id)) || 
        (anims::_check_if_entity_in_anim(entity_id) & 
                !anims::_check_if_entity_in_anim_same_type(anim_id, entity_id) &
                anims::animsplayed[anim_id].breakable))
      {
        std::cout << "Starting animation " << anim_id << "for entity " << entity_id << std::endl;
        textures::Animation AD  = textures::textures[entity::entities[entity_id].texture_id].anims[anim_id];
        AD.entity_id = entity_id;
        AD.time_elapsed = 0.0f;
        AD.current_keyframe_index = 0;
        AD.next_update_time = AD.update_times[1];
        AD.start_time = timer::get_current_hrc_time();
        anims::animsplayed[entity_id] = AD;
        entity::update_frame(entity_id, AD.frame_id[AD.current_keyframe_index]);
      }
    }
  }

  void play(int entity_id)
  { 
    anims::animsplayed[entity_id].time_elapsed = timer::get_elapsed_time(anims::animsplayed[entity_id].start_time);

    // Update frame if its passed the next time threshold and is under animation time
    if(anims::animsplayed[entity_id].time_elapsed >= anims::animsplayed[entity_id].next_update_time & 
      anims::animsplayed[entity_id].time_elapsed  <= anims::animsplayed[entity_id].time_length)
    {
      anims::animsplayed[entity_id].current_keyframe_index += 1;
      anims::animsplayed[entity_id].next_update_time = anims::animsplayed[entity_id].update_times[(anims::animsplayed[entity_id].current_keyframe_index+1)];

      // later will be replaced by map of functions I believe
      int frame_id = anims::animsplayed[entity_id].frame_id[anims::animsplayed[entity_id].current_keyframe_index];
      entity::update_frame(entity_id, frame_id);
    }

    if(anims::animsplayed[entity_id].time_elapsed > anims::animsplayed[entity_id].time_length)
    {
      anims::anims_to_stop.push_back(entity_id);
    }
  };


  void update()
  {
    anims::anims_to_stop.clear();

    for (auto & [k, v] : anims::animsplayed)
    {
      anims::play(k);
    }

    for(int a=0; a<anims::anims_to_stop.size(); a++)
    {
      anims::drop(anims::anims_to_stop[a]);
    }

  };

  bool _check_if_entity_in_anim(int entity_id)
  {
    bool has_anim = false;
    if(anims::animsplayed.count(entity_id) > 0)
    {
      has_anim = true;
    }
    return has_anim;
  }

  bool _check_if_entity_in_anim_same_type(int anim_id, int entity_id)
  {
    bool has_anim_same_type = false;
    if(anims::animsplayed.count(entity_id) > 0)
    {
      if(anims::animsplayed[entity_id].id == anim_id)
      {
        has_anim_same_type = true;
      }
    }
    return has_anim_same_type;
  }

}

