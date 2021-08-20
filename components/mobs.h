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
  }

  void spawn(int map_id)
  { 
    // takes nests data from maps catalog using map id
    // spawns mobs on the level based on the level information
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
      }
    };
  }








}

#endif