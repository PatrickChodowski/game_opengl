#ifndef ANIMS_H
#define ANIMS_H

// it should be similar, but shorter than TravelPlans
namespace anims
{
    struct PlayedAnimation
    {
        int event_id;
        int quad_id;
        int current_frame;
        int next_frame;
        float delay;

        auto frame_update_time = std::chrono::system_clock::now();
        float time_since_last_update;

    };


    void set_frame(int quad_index, int frame_id)
    {

    }

    void play(int quad_index, int event_id)
    {
        // check if event_id exists in given texture animation catalog:
        if(textures::Catalog[quads::AllQuads[quad_index].texture_id].anims.count(event_id) > 0)
        {
            float time_since_last_update = timer::get_elapsed_time(quads::AllQuads[quad_index].frame_update_time);



        }

    }

    void manage()
    {

    }


}




#endif 