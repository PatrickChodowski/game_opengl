#include <iostream>
#include <string>
#include <vector>

#include "entity.h"
#include "logger.h"
#include "maps.h"
#include "npcs.h"
#include "utils.h"

#include "../dependencies/json_struct.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace npcs
{
  std::vector<int> Index;
  phmap::flat_hash_map<int, NPCData> npcs_data;
  phmap::flat_hash_map<int, NPCData> npcs;
  std::vector<npcs::InteractionData> interactions;

  void read_data(std::string& name)
  {
    npcs::NPCData ND;
    std::string data_path = "./data/npcs/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(ND);
    npcs::npcs_data.insert({ND.id, ND});
  };

  void init()
  {
    std::vector<std::string> npc_list = utils::list_json_files("data/npcs");
    for(int n=0; n < npc_list.size(); n++)
    {
      npcs::read_data(npc_list[n]);
    };
    std::cout << "NPCs Initialized" << std::endl;
  };

  int spawn(int npc_id, float x, float y)
  {
    npcs::NPCData ndd = npcs::npcs_data[npc_id];
    ndd.x = x;
    ndd.y = y;
    int entity_id = entity::create(ndd, ENTITY_TYPE_NPC, CAMERA_DYNAMIC);
    ndd.entity_id = entity_id;

    npcs::npcs[entity_id] = ndd;
    return entity_id;
  }

  void spawn_from_map(int map_id)
  {
    if(map_id > -1)
    { 
      for(int n = 0; n < maps::maps[map_id].npcs.size(); n++)
      {
        npcs::spawn(maps::maps[map_id].npcs[n].id, maps::maps[map_id].npcs[n].x, maps::maps[map_id].npcs[n].y);
      }
    }
  };

  void clear()
  {
    npcs::npcs.clear();
  };

  void refresh()
  {
    npcs::npcs_data.clear();
    npcs::interactions.clear();
  };


  void interact(int entity_id, float value)
  {
    if(npcs::npcs.count(entity_id) > 0)
    {
      npcs::InteractionData IDD;
      IDD.event_type_id = 0;
      IDD.npc_id = entity_id;
      IDD.type = 1.0f;
      IDD.value = value;
      npcs::interactions.push_back(IDD);

      npcs::npcs[entity_id].sentiment += value;
      std::cout << "NPC: " << entity_id << " Sentiment is now at: " << npcs::npcs[entity_id].sentiment << std::endl;
    }
  }

  std::vector<std::string> info(int entity_id)
  {
    std::vector<std::string> infos = {};
    infos.push_back(npcs::npcs[entity_id].name);
    return infos;
  }

}
