
#include "anims.h"
#include "entity.h"
#include "utils.h"
#include "timer.h"

#include "../dependencies/json_struct.h"

namespace anims
{
  std::map<int, anims::Animation> anims;
  std::map<int, anims::Animation> animsplayed;

  void init()
  {
    std::vector<std::string> anim_list = utils::list_json_files("data/anims");
    for(int a=0; a < anim_list.size(); a++)
    {
      anims::read_data(anim_list[a]);
    };
  }

  void read_data(std::string& name)
  {
    anims::Animation AD;
    std::string data_path = "./data/anims/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(AD);
    anims::anims.insert({AD.id, AD});
  };

  void clear()
  {
    anims::animsplayed.clear();
  }

  void drop(int entity_id)
  {
    anims::animsplayed.erase(entity_id);
  }

  void refresh()
  {
    anims::anims.clear();
  }

  void update()
  {
    
  }

  bool check_if_entity_in_anim(int entity_id)
  {
    bool has_anim = false;
    if(anims::animsplayed.count(entity_id) > 0)
    {
      has_anim = true;
    }
    return has_anim;
  }

}

