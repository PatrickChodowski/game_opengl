#ifndef MOBS_H
#define MOBS_H


namespace mobs
{
  
  struct MobData
  {
    int id;
    std::string type;
    int w;
    int h;
    int texture_id;
    int min_dmg;
    int max_dmg;
    int min_speed;
    int max_speed;
    int min_hp;
    int max_hp;
    int min_def;
    int max_def;

    JS_OBJ(id, type, w, h, texture_id, min_dmg, max_dmg, min_speed,
    max_speed, min_hp, max_hp, min_def, max_def);
  };

  std::map<int, MobData> Catalog = {};

  void read_mob_data(std::string name)
  {
    MobData MD;
    std::string data_path = "./mobs/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(MD);
    Catalog.insert({MD.id, MD});
  }

  void init()
  {
    logger::print("mobs::init(): READING MOBS");
    std::vector<std::string> mob_list = utils::list_files("mobs/");
    for(int m=0; m < mob_list.size(); m++)
    {
      read_mob_data(mob_list[m]);
    }
  }

  void drop()
  {
    mobs::Catalog.clear();
    mobs::AliveMobs.clear();
  }

  void spawn(int map_id)
  { 
    // takes nests data from maps catalog using map id
    // spawns mobs on the level based on the level information
    AliveMobs.clear();
    int MOB_ID = 0;
    // unpack each nest
    for(int i=0; i < maps::Catalog[map_id].nests.size(); i++)
    {
      for(int m=0; m<maps::Catalog[map_id].nests[i].n; m++)
      {
        quads::Quad mob_quad = ent::render_entity(ENTITY_TYPE_ID_MOB,
                                                  true,
                                                  mobs::Catalog[MOB_ID].texture_id,
                                                  0,
                                                  maps::Catalog[map_id].nests[i].x + (m*mobs::Catalog[MOB_ID].w),
                                                  maps::Catalog[map_id].nests[i].y,
                                                  mobs::Catalog[MOB_ID].h,
                                                  mobs::Catalog[MOB_ID].w,
                                                  0.0f,
                                                  textures::FontTD,
                                                  true,
                                                  true
                                                );
        ent::EntityQuads.push_back(mob_quad);

        struct AliveMobData amd;
        amd.quad_id = mob_quad.id;
        amd.x = mob_quad.x;
        amd.y = mob_quad.y;
        amd.mob_id = MOB_ID;
        amd.speed = mobs::Catalog[MOB_ID].max_speed;
        amd.hp = mobs::Catalog[MOB_ID].max_hp;
        AliveMobs.push_back(amd);
      }
    };
  }


  void move_random()
  { 
    for (int a=0; a<mobs::AliveMobs.size(); a++)
    {
      if(mobs::AliveMobs[a].state == ENTITY_STATE_CALM)
      {
        int r = utils::get_random(0,3);
        if(r == 0){
          mobs::AliveMobs[a].x += 1;
        } else if (r==1) {
          mobs::AliveMobs[a].x -= 1;
        } else if (r==2) {
          mobs::AliveMobs[a].y -= 1;
        } else if (r==3) {
          mobs::AliveMobs[a].y += 1;
        }
        // int mob_node_id = paths::get_navnode_id(mobs::AliveMobs[a].x, mobs::AliveMobs[a].y);
      }
    }
  }

  void move_to_point(float x, float y)
  {
    // get mob quad id (temporary):
    int quad_index; 
    for(int q = 0; q < quads::AllQuads.size(); q++)
    {
      if(quads::AllQuads[q].entity_type_id == ENTITY_TYPE_ID_MOB)
      {
        quad_index = q;
        break;
      }
    }
    std::cout << "Mob Quad Index: " << quad_index << std::endl;
    std::cout << "Mob Position: " << quads::AllQuads[quad_index].s_x << "," << quads::AllQuads[quad_index].s_y << std::endl;

    // int quad_index = quads::find_quad_id(quad_id, quads::AllQuads);
    int quad_node_id = paths::get_navnode_id(quads::AllQuads[quad_index].s_x, quads::AllQuads[quad_index].s_y);
    // std::cout << "Mob Polygon: " << quad_node_id << std::endl;


    int target_node_id = paths::get_navnode_id(x, y);
    if(quad_node_id != target_node_id)
    {
      paths::find_path(quad_node_id, target_node_id);
    }
  }

  void switch_aggro()
  {
    for (int a=0; a<mobs::AliveMobs.size(); a++)
    {
      if(mobs::AliveMobs[a].state == ENTITY_STATE_CALM)
      {
        mobs::AliveMobs[a].state = ENTITY_STATE_MOVING;
      } 
      else if (mobs::AliveMobs[a].state == ENTITY_STATE_MOVING)
      {
        mobs::AliveMobs[a].state = ENTITY_STATE_CALM;
        if(travel::TravelControl.count(mobs::AliveMobs[a].quad_id) > 0)
        {
          travel::TravelControl.erase(mobs::AliveMobs[a].quad_id);  
        }
      }
    }
  }

  void move_aggro_mobs_to_point(float x, float y)
  {
    for (int a=0; a < mobs::AliveMobs.size(); a++)
    {
      if(mobs::AliveMobs[a].state == ENTITY_STATE_MOVING)
      { 
        int quad_index = quads::find_quad_id(mobs::AliveMobs[a].quad_id, quads::AllQuads);
        std::cout << "Mob Quad Index: " << quad_index << std::endl;
        std::cout << "Mob Position: " << quads::AllQuads[quad_index].s_x << "," << quads::AllQuads[quad_index].s_y << std::endl;
        int quad_node_id = paths::get_navnode_id(quads::AllQuads[quad_index].s_x, quads::AllQuads[quad_index].s_y);
        int target_node_id = paths::get_navnode_id(x, y);

        std::cout << "quad_node_id: " << quad_node_id << std::endl;
        std::cout << "target_node_id: " << target_node_id << std::endl;
        travel::TravelPlan tp;
        if(quad_node_id != target_node_id)
        {
          std::vector<int> path = paths::find_path(quad_node_id, target_node_id);
          tp.quad_id = mobs::AliveMobs[a].quad_id;
          tp.full_path = path;
          tp.current_node = quad_node_id;
          tp.next_node = path[1];
          tp.target_node = target_node_id;
          tp.current_x = quads::AllQuads[quad_index].s_x;
          tp.current_y = quads::AllQuads[quad_index].s_y;
          tp.target_x = x;
          tp.target_y = y;

        } else if (quad_node_id == target_node_id){
          tp.quad_id = mobs::AliveMobs[a].quad_id;
          tp.full_path = {};
          tp.current_node = quad_node_id;
          tp.next_node = target_node_id;
          tp.target_node = target_node_id;
          tp.current_x = quads::AllQuads[quad_index].s_x;
          tp.current_y = quads::AllQuads[quad_index].s_y;
          tp.target_x = x;
          tp.target_y = y;
        }
        travel::TravelControl.insert({tp.quad_id, tp});
      }
    }
  }


  void render_alive_mobs()
  {
    for (int a=0; a<mobs::AliveMobs.size(); a++)
    {
      quads::Quad mob_quad = ent::render_entity(ENTITY_TYPE_ID_MOB,
                                                true,
                                                mobs::Catalog[mobs::AliveMobs[a].mob_id].texture_id,
                                                0,
                                                mobs::AliveMobs[a].x,
                                                mobs::AliveMobs[a].y,
                                                mobs::Catalog[mobs::AliveMobs[a].mob_id].h,
                                                mobs::Catalog[mobs::AliveMobs[a].mob_id].w,
                                                0.0f,
                                                textures::FontTD,
                                                true,
                                                true
                                              );
      ent::EntityQuads.push_back(mob_quad);
      mobs::AliveMobs[a].quad_id = mob_quad.id;
    }
  }



}

#endif