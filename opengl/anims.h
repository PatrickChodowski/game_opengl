#ifndef ANIMS_H
#define ANIMS_H

// it should be similar, but shorter than TravelPlans
namespace anims
{
    bool check_if_entity_in_played_anim(int entity_id)
    {
      bool check;
      if(anims::PlayAnimationControl.count(entity_id) > 0)
      {
        check = true;
      } else {
        check = false;
      }
      return check;
    }


    // Starts animation - creates PlayAnimation object for entity and provides event_id which tells what animation to start
    void start(int entity_id, int event_id, bool breakable = true)
    {
      bool check_in_anim = check_if_entity_in_played_anim(entity_id);
      bool breakable_anim = false;
      if(check_in_anim)
      {
        breakable_anim = anims::PlayAnimationControl[entity_id].breakable;
      }

      // not in animation or in animation that  is breakable
      if(!check_in_anim | (check_in_anim & breakable_anim))
      {
        anims::PlayAnimation pa;
        pa.entity_id = entity_id;
        pa.event_id = event_id;
        pa.breakable = breakable;

        pa.frame_update_time = timer::get_current_ms_time();
        pa.time_since_last_update = 0.0f;
        pa.texture_id = -1;
        pa.seq_index = 0;

        // texture ID from Entity Quads
        // but need to update frame in AllQuads -> this has to be done before summarizing quads
        int ent_quad_index = ent::find_entity(entity_id);
        pa.texture_id = ent::EntityQuads[ent_quad_index].texture_id;
        pa.quad_id = ent::EntityQuads[ent_quad_index].id;


        // if found texture, read the anims data from texture catalog
        if(pa.texture_id > -1)
        {
          // check if given animation exists in the texture
          if(textures::Catalog[pa.texture_id].anims.count(event_id) > 0)
          {
            pa.animation_label = textures::Catalog[pa.texture_id].anims[event_id].label;
            // pa.current_frame = textures::Catalog[pa.texture_id].anims[event_id].label;

            // (NOTE): Somehow need to pass current frame of the entity 
            // to decide which sequence to fire
            // or not? just finish the animation
            // what If I finish the move before (anim will go right and I will change left)
            // is it the long sequence or short sequence.
            // shall I lock the direction till it finishes?

            pa.current_frame = textures::Catalog[pa.texture_id].anims[event_id].sequence[pa.seq_index].id;
            pa.next_frame = textures::Catalog[pa.texture_id].anims[event_id].sequence[pa.seq_index].next;
            pa.delay = textures::Catalog[pa.texture_id].anims[event_id].sequence[pa.seq_index].ms_delay;

          }
        }
        if(breakable_anim){
          // overwriting breakable anim for this entity
          anims::PlayAnimationControl[entity_id] = pa;
        } else {
          // othwerwise inserting
          anims::PlayAnimationControl.insert({entity_id, pa});
        }
      }
        // return pa;
        //this belongs to entity logic? 
        //anims::PlayAnimationControl.insert({anims::AliveMobs[a].entity_id, pa});
    }

    // plays animation: check for elapsed time
    void play(anims::PlayAnimation pa)
    {
      
      //std::cout << "playing animation" << std::endl;
      std::chrono::milliseconds now_time = timer::get_current_ms_time();
      pa.time_since_last_update = timer::get_elapsed_ms(pa.frame_update_time);

      if(pa.time_since_last_update >= pa.delay)
      {
        pa.frame_update_time = now_time;
        std::cout << "time since last update: " << pa.time_since_last_update << std::endl;
      }



      int ent_quad_index = ent::find_entity(pa.entity_id);
      

    }

    // manages played animations
    void manage()
    {
        PAsToRemove.clear();
        for (auto const& pa : anims::PlayAnimationControl)
        {  
            anims::play(pa.second);
        }
        for(int i=0; i < PAsToRemove.size(); i++)
        {
            if(anims::PlayAnimationControl.count(PAsToRemove[i]) > 0)
            {
                std::cout << "removing " <<  PAsToRemove[i] << " from Played Animations" << std::endl;  
                anims::PlayAnimationControl.erase(PAsToRemove[i]);  
            }
        }
    }

}

#endif 
