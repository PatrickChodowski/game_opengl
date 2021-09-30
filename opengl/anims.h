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
    void start(int entity_id, int event_id, bool breakable)
    {
      bool green;
      // first check if given entity is already in animation
      if(check_if_entity_in_played_anim(entity_id))
      {
        // if it is in animation, check if its breakable animation
        if(anims::PlayAnimationControl[entity_id].breakable)
        {


        }

      }


        anims::PlayAnimation pa;
        pa.entity_id = entity_id;
        pa.event_id = event_id;
        pa.breakable = breakable;

        pa.frame_update_time = timer::get_current_time();
        pa.time_since_last_update = 0.0f;
        pa.texture_id = -1;
        pa.seq_index = 0;

        // texture ID from Entity Quads
        // but need to update frame in AllQuads -> this has to be done before summarizing quads

        for(int e=0; e<ent::EntityQuads.size(); e++)
        {
          if(entity_id == ent::EntityQuads[e].entity_id)
          {
            pa.texture_id = ent::EntityQuads[e].texture_id;
            pa.quad_id = ent::EntityQuads[e].id;
            break;
          }
        }

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
            pa.delay = textures::Catalog[pa.texture_id].anims[event_id].sequence[pa.seq_index].time;

          }


        }
        anims::PlayAnimationControl.insert({entity_id, pa});
        // return pa;
        //this belongs to entity logic? 
        //anims::PlayAnimationControl.insert({anims::AliveMobs[a].entity_id, pa});
    }

    void play(anims::PlayAnimation pa)
    {
      pa.
      

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
