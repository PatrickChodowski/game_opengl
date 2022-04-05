
#include <iostream>

#include "anims.h"
#include "entity.h"
#include "models.h"
#include "utils.h"
#include "timer.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace anims
{
  phmap::flat_hash_map<int, models::ModelAnimData> animsplayed;
  phmap::flat_hash_map<int, sig_ptr> AnimsHandler;
  std::vector<int> anims_to_stop;

  void init()
  {
    anims::AnimsHandler[ANIM_TYPE_FRAME] = anims::update_frame;
    anims::AnimsHandler[ANIM_TYPE_FRAME_POSITION] = anims::update_frame_position;
  }


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
    // Check if given entity's model has this animation id available in the first place
    if(models::models[entity::entities[entity_id].model_id].anims.count(anim_id) > 0)
    {
      if((!anims::_check_if_entity_in_anim(entity_id)) || 
        (anims::_check_if_entity_in_anim(entity_id) & 
                !anims::_check_if_entity_in_anim_same_type(anim_id, entity_id) &
                anims::animsplayed[entity_id].breakable))
      {
        models::ModelAnimData AD  = models::models[entity::entities[entity_id].model_id].anims[anim_id];
        AD.id = anim_id;
        AD.entity_id = entity_id;
        AD.time_elapsed = 0.0f;
        AD.CK_ID = 0; // current keyframe index
        AD.next_update_time = AD.update_times[1];
        AD.start_time = timer::get_current_hrc_time();
        anims::animsplayed[entity_id] = AD;
        anims::AnimsHandler[AD.anim_type_id](entity_id, AD);
      }
    }
  }

  void play(int entity_id)
  { 
    //std::cout << "playing animation for " << entity_id << std::endl;
    anims::animsplayed[entity_id].time_elapsed = timer::get_elapsed_time_ms(anims::animsplayed[entity_id].start_time);  

    // Update frame if its passed the next time threshold and is under animation time
    if(anims::animsplayed[entity_id].time_elapsed >= anims::animsplayed[entity_id].next_update_time & 
       anims::animsplayed[entity_id].time_elapsed  <= anims::animsplayed[entity_id].time_length)
    {
      anims::animsplayed[entity_id].CK_ID += 1;
      anims::animsplayed[entity_id].next_update_time = anims::animsplayed[entity_id].update_times[(anims::animsplayed[entity_id].CK_ID+1)];
      anims::AnimsHandler[anims::animsplayed[entity_id].anim_type_id](entity_id, anims::animsplayed[entity_id]);
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
      int entity_id = anims::anims_to_stop[a];
      int next_anim_id = anims::animsplayed.at(entity_id).next_anim_id;
      anims::drop(entity_id);

      if(next_anim_id > -1){
        anims::start(next_anim_id, entity_id);
      }
    }
  };


  void start_delayed(int anim_id, int entity_id)
  {
    // Check if given entity's model has this animation id available in the first place
    if(models::models[entity::entities[entity_id].model_id].anims.count(anim_id) > 0)
    {
      if(anims::_check_if_entity_in_anim(entity_id))
      {
        anims::animsplayed.at(entity_id).next_anim_id = anim_id;
      }
    }
  }



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




  // Anim handlers:

  void update_frame(int entity_id, models::ModelAnimData &AD)
  {
    int model_id = entity::entities.at(entity_id).model_id;
    int side_id = entity::entities.at(entity_id).side_id;

    // Frame ID formula
    // Final frame_id = ANIM_ID*10000 + SIDE_ID*100 + FRAME_ID
    // frame_id = self.id*10000 + side_id*100 + img_data.frame_index

    int frame_id = AD.anim_id*10000 + entity::entities.at(entity_id).side_id*100 + AD.frame_id[AD.CK_ID];
    entity::entities.at(entity_id).frame_id = frame_id;
  }

  void update_frame_position(int entity_id, models::ModelAnimData &AD)
  {
    anims::update_frame(entity_id, AD);
    entity::entities.at(entity_id).pos.z = AD.z[AD.CK_ID];
  }


}

