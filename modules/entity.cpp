#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "entity.h"
#include "fonts.h"
#include "hero.h"
#include "items.h"
#include "mobs.h"
#include "quads.h"
#include "textures.h"
#include "utils.h"
#include "../dictionary.h"

namespace entity
{

  std::map<int, EntityData> entities;
  std::vector<int> Index = {};
  std::vector<quads::QuadData> EntityQuads = {};

  template <typename T>
  int create(T data, int entity_type_id, float camera_type)
  { 
    entity::EntityData edd;
    edd.id = utils::generate_id(entity::Index);
    edd.texture_id = data.texture_id;
    edd.frame_id = data.current_frame;
    edd.entity_type_id = entity_type_id;
    edd.x = data.x;
    edd.y = data.y;
    edd.h = data.h;
    edd.w = data.w;
    edd.prev_x = data.x;
    edd.prev_y = data.y;
    edd.mid_x = edd.x + (edd.w/2);
    edd.mid_y = edd.y + (edd.h/2);
    edd.diag = std::sqrt(std::pow((edd.w/2),2) + std::pow((edd.h/2),2));
    edd.camera_type = camera_type;
    edd.is_solid = false;
    if(entity_type_id == ENTITY_TYPE_MOB || entity_type_id == ENTITY_TYPE_HERO)
    {
      edd.is_solid = true;
    }
    edd.is_clicked = false;
    edd.speed = data.speed;

    edd.norm_x_start = textures::_get_normalized_frame_start(data.texture_id, data.current_frame);
    edd.norm_x_end = textures::_get_normalized_frame_end(data.texture_id, data.current_frame);

    entity::entities[edd.id] = edd;
    return edd.id;
  }

  void render()
  {
    entity::EntityQuads.clear();
    entity::EntityQuads = quads::make_quads(entity::entities, OBJECT_TYPE_ENTITY);
  };

  void clear()
  {
    entity::Index.clear();
    entity::entities.clear();
    entity::EntityQuads.clear();
  };

  // int display(int entity_id, int gui_slot_id)
  // {
  //   std::string entity_info;
  //   entity::EntityData edd = entity::entities[entity_id];
  //   entity_info = "Entity ID: " + utils::str(entity_id);
  //   int label_id = fonts::add(entity_info, 
  //               gui::guislots[gui_slot_id].x + 5, 
  //               gui::guislots[gui_slot_id].y + 50, 
  //               CAMERA_STATIC, 
  //               0.7,
  //               0.0f, 0.0f, 0.0f);
  //   return label_id;
  // };

  template int entity::create<hero::HeroData>(hero::HeroData, int, float);
  template int entity::create<items::ItemData>(items::ItemData, int, float);
  template int entity::create<mobs::MobData>(mobs::MobData, int, float);

}
