#include <map>
#include <string>
#include <vector>

#include "entity.h"
#include "npcs.h"
#include "utils.h"

#include "../dependencies/json_struct.h"

namespace npcs
{
  std::vector<int> Index;
  std::map<int, NPCData> npcs_data;
  std::map<int, NPCData> npcs;
  std::vector<InteractionData> interactions;

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
  };

  void clear()
  {
    npcs::npcs.clear();
    npcs::Index.clear();
  };

  void refresh()
  {
    npcs::npcs_data.clear();
  };

  void drop(int npc_id)
  {
    npcs::npcs.erase(npc_id);
    utils::drop_id(npcs::Index, npc_id);
  };

}
