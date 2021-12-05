
#include <iostream>

#include "anims.h"
#include "entity.h"
#include "utils.h"
#include "timer.h"

#include "../dependencies/json_struct.h"

namespace anims
{
  std::map<int, anims::Animation> anims;
  std::map<int, anims::Animation> animsplayed;
  std::vector<int> anims_to_stop;

  void init()
  {
    std::vector<std::string> anim_list = utils::list_json_files("data/anims");
    for(int a=0; a < anim_list.size(); a++)
    {
      anims::read_data(anim_list[a]);
    };
  }

  void read_data(std::string& name)
  {
    anims::Animation AD;
    std::string data_path = "./data/anims/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(AD);
    anims::anims.insert({AD.id, AD});
  };

  void clear()
  {
    anims::animsplayed.clear();
  }

  void drop(int entity_id)
  {
    anims::animsplayed.erase(entity_id);
  }

  void refresh()
  {
    anims::anims.clear();
  }

  void start(int anim_type_id, int entity_id)
  {
    // If entity not in animation right now
    // Later: check if its the same type, check if breakable and can be replaced
    if(!_check_if_entity_in_anim(entity_id))
    {
      anims::Animation AD  = anims::anims[anim_type_id];
      AD.entity_id = entity_id;
      AD.time_elapsed = 0.0f;

      AD.current_frame_index = 0;
      AD.next_e_time = AD.frame_times[1];

      AD.start_time = timer::get_current_hrc_time();
      anims::animsplayed[entity_id] = AD;
      entity::update_frame(entity_id, AD.frame_ids[AD.current_frame_index]);
    }
    //question to send frame tp entity now?
  }


  void play(int entity_id)
  { 
    anims::animsplayed[entity_id].time_elapsed = timer::get_elapsed_time(anims::animsplayed[entity_id].start_time);

    // Update frame if its passed the next time threshold and is under animation time
    if(anims::animsplayed[entity_id].time_elapsed >= anims::animsplayed[entity_id].next_e_time & 
      anims::animsplayed[entity_id].time_elapsed  <= anims::animsplayed[entity_id].time_length)
    {
      anims::animsplayed[entity_id].current_frame_index += 1;
      anims::animsplayed[entity_id].next_e_time = anims::animsplayed[entity_id].frame_times[(anims::animsplayed[entity_id].current_frame_index+1)];

      // later will be replaced by map of functions I believe
      int frame_id = anims::animsplayed[entity_id].frame_ids[anims::animsplayed[entity_id].current_frame_index];
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

  bool _check_if_entity_in_anim_same_type(int anim_type_id, int entity_id)
  {
    bool has_anim_same_type = false;
    if(anims::animsplayed.count(entity_id) > 0)
    {
      if(anims::animsplayed[entity_id].id == anim_type_id)
      {
        has_anim_same_type = true;
      }
    }
    return has_anim_same_type;
  }

}

