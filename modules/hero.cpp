#include "hero.h"

namespace hero2
{

  hero2::HeroData hero;

  void _load_from_save(std::string save_name)
  {
    std::string file_path = "saves/" + save_name + ".json";
    std::string json_data = utils2::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero2::hero);
  };


}
