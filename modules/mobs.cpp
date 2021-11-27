
#include "entity.h"
#include "maps.h"
#include "mobs.h"
#include "quads.h"
#include "utils.h"

#include "../dictionary.h"

#include <map>
#include <iostream>
#include <string>
#include <vector>

namespace mobs
{

  std::map<int, MobData> mobs = {};

  void read_data(std::string name)
  {
    mobs::MobData MD;
    std::string data_path = "./data/mobs/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    mobs::mobs.insert({MD.id, MD});
  };


  void init()
  {
    std::vector<std::string> mob_list = utils::list_json_files("./data/mobs/");
    for(int m=0; m < mob_list.size(); m++)
    {
      mobs::read_data(mob_list[m]);
    }
  };

  void clear()
  {
    mobs::mobs.clear();
  }


  void spawn(int map_id)
  {
    int MOB_ID = 0;
    // unpack each nest
    for(int i=0; i < maps::maps[map_id].nests.size(); i++)
    {
      // for each mob of the nest
      for(int m=0; m<maps::maps[map_id].nests[i].n; m++)
      {
        mobs::MobData mdd = mobs::mobs[MOB_ID];
        mdd.x = maps::maps[map_id].nests[i].x;
        mdd.y = maps::maps[map_id].nests[i].y;
        mdd.current_frame = 0;

        // logic for creating alive entity?

        entity::create(mdd, OBJECT_TYPE_ENTITY, CAMERA_DYNAMIC);
      }
    }
  };


  // void move_random()
  // { 
  //   for (int a=0; a<mobs::AliveMobs.size(); a++)
  //   {
  //     if(mobs::AliveMobs[a].state == ENTITY_STATE_CALM)
  //     {
  //       int r = utils::get_random(0,3);
  //       if(r == 0){
  //         mobs::AliveMobs[a].x += 1;
  //       } else if (r==1) {
  //         mobs::AliveMobs[a].x -= 1;
  //       } else if (r==2) {
  //         mobs::AliveMobs[a].y -= 1;
  //       } else if (r==3) {
  //         mobs::AliveMobs[a].y += 1;
  //       }
  //       // int mob_node_id = paths::get_navnode_id(mobs::AliveMobs[a].x, mobs::AliveMobs[a].y);
  //     }
  //   }
  // }

  // void switch_aggro()
  // {
  //   for (int a=0; a<mobs::AliveMobs.size(); a++)
  //   {
  //     if(mobs::AliveMobs[a].state == ENTITY_STATE_CALM)
  //     {
  //       mobs::AliveMobs[a].state = ENTITY_STATE_MOVING;
  //     } 
  //     else if (mobs::AliveMobs[a].state == ENTITY_STATE_MOVING)
  //     {
  //       mobs::AliveMobs[a].state = ENTITY_STATE_CALM;
  //       if(travel::TravelControl.count(mobs::AliveMobs[a].quad_id) > 0)
  //       {
  //         travel::TravelControl.erase(mobs::AliveMobs[a].quad_id);  
  //       }
  //     }
  //   }
  // }

  // void move_aggro_mobs_to_point(float x, float y)
  // {
  //   float target_rs_x = camera::reverse_scale_click_x(x, camera::x, camera::zoom);
  //   float target_rs_y = camera::reverse_scale_click_y(y, camera::y, camera::zoom);

  //   for (int a=0; a < mobs::AliveMobs.size(); a++)
  //   {
  //     if(mobs::AliveMobs[a].state == ENTITY_STATE_MOVING)
  //     { 
  //       // if(travel::check_if_entity_already_moving(mobs::AliveMobs[a].entity_id))
  //       // {
  //       //   travel::cancel_travel_plan(mobs::AliveMobs[a].entity_id);
  //       // }

  //       int quad_index = quads::find_quad_id(mobs::AliveMobs[a].quad_id, quads::AllQuads);
  //       int quad_node_id = paths::get_navnode_id(quads::AllQuads[quad_index].x, quads::AllQuads[quad_index].y);
  //       int target_node_id = paths::get_navnode_id_scaled(x, y);

  //       if(quad_node_id > -1 & target_node_id > -1){

  //         travel::TravelPlan tp = travel::make_basic_plan(quad_node_id, target_node_id);
  //         tp.quad_id = mobs::AliveMobs[a].quad_id;
  //         tp.current_x = quads::AllQuads[quad_index].x;
  //         tp.current_y = quads::AllQuads[quad_index].y;
  //         tp.target_x = target_rs_x;
  //         tp.target_y = target_rs_y;
  //         tp.entity_id = mobs::AliveMobs[a].entity_id;
  //         travel::TravelControl.insert({mobs::AliveMobs[a].entity_id, tp});
  //         // float dist_to_target = travel::get_distance_between_points(tp.current_x, tp.current_y, tp.target_x, tp.target_y); 
  //       }
  //     }
  //   }
  // }
  
}
