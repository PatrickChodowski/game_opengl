#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "logger.h"
#include "maps.h"
#include "npcs.h"
#include "utils.h"

#include "../dependencies/json_struct.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"

namespace npcs
{
  std::vector<npcs::InteractionData> interactions;
  phmap::flat_hash_map<int, npcs::NPCData> npcs = {};

  void init()
  {
    std::vector<std::string> npc_list = utils::list_json_files("./data/npcs/");
    for(int n=0; n < npc_list.size(); n++)
    {
      npcs::read_data(npc_list[n]);
    }
    std::cout << "NPCs Initialized" << std::endl;
  };

  void read_data(std::string name)
  {
    npcs::NPCData ND;
    std::string data_path = "./data/npcs/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(ND);
    npcs::npcs.insert({ND.npc_id, ND});
  };

  void refresh()
  {
    npcs::interactions.clear();
    npcs::npcs.clear();
  };

  void interact(int entity_id, float value)
  {
    if(ecs::npcs.count(entity_id))
    {
      npcs::InteractionData IDD;
      IDD.event_type_id = 0;
      IDD.npc_id = entity_id;
      IDD.type = 1.0f;
      IDD.value = value;
      //ecs::npcs.at(entity_id).interactions.push_back(IDD);
      npcs::interactions.push_back(IDD);
      ecs::npcs.at(entity_id).sentiment += value;
      std::cout << " [NPC] " << entity_id << " Sentiment is now at: " << ecs::npcs.at(entity_id).sentiment << std::endl;
    }
  }

}
