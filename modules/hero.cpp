#include "entity.h"
#include "hero.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"


namespace hero
{

  hero::HeroData hero;

  void _read_data(std::string char_type)
  {
    std::string file_path = "data/heros/" + char_type + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero::hero);
  }

  void _load_from_save(std::string save_name)
  {
    std::string file_path = "saves/" + save_name + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero::hero);
  };

  void create_new(std::string name, std::string type)
  {
    hero::_read_data(type);
    hero.current_frame = 0;
    hero.current_movement_state = ENTITY_STATE_CALM;
    hero.name = name;
    hero.type = type;
    entity::create(hero::hero, OBJECT_TYPE_ENTITY, CAMERA_ZOOM_ONLY);
  };


}
