#ifndef ANIMS_H
#define ANIMS_H

// it should be similar, but shorter than TravelPlans
namespace anims
{

    void set_frame(int quad_index, int frame_id)
    {

    }

    // Starts animation - creates PlayAnimation object for entity and provides event_id which tells what animation to start
    anims::PlayAnimation start(int entity_id, int event_id)
    {
        anims::PlayAnimation pa;
        pa.entity_id = entity_id;
        pa.entity_id = event_id;
        pa.quad_id = ent::EntityQuadList[entity_id];

        // frames should come from texture catalog
        pa.current_frame = 0;
        pa.next_frame = 0;
        pa.delay = 0.0;
        
        pa.frame_update_time = timer::get_current_time();
        pa.time_since_last_update = 0.0f;

        //
        // pa.texture_id = ?? // do i need it?







        return pa;
        //this belongs to entity logic? 
        //anims::PlayAnimationControl.insert({anims::AliveMobs[a].entity_id, pa});
    }

    void play(anims::PlayAnimation pa)
    {
        // check if event_id exists in given texture animation catalog:
        // if(textures::Catalog[quads::AllQuads[quad_index].texture_id].anims.count(event_id) > 0)
        // {
        //     //float time_since_last_update = timer::get_elapsed_time(quads::AllQuads[quad_index].frame_update_time);



        // }

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
