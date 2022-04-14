#include <iostream>
#include <string>
#include <vector>

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

  void init()
  {
    std::cout << "NPCs Initialized" << std::endl;
  };

  void refresh()
  {
    npcs::interactions.clear();
  };

  void clear()
  {};

  void interact(int entity_id, float value)
  {
    // if(npcs::npcs.count(entity_id) > 0)
    // {
    //   npcs::InteractionData IDD;
    //   IDD.event_type_id = 0;
    //   IDD.npc_id = entity_id;
    //   IDD.type = 1.0f;
    //   IDD.value = value;
    //   npcs::interactions.push_back(IDD);

    //   npcs::npcs[entity_id].sentiment += value;
    //   std::cout << "NPC: " << entity_id << " Sentiment is now at: " << npcs::npcs[entity_id].sentiment << std::endl;
    // }
  }

}
