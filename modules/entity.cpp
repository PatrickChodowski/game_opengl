#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <ostream>
#include <vector>

#include "entity.h"
#include "hero.h"
#include "items.h"
#include "mobs.h"
#include "quads.h"
#include "utils.h"
#include "../dictionary.h"

namespace entity
{

  std::map<int, EntityData> entities;
  std::vector<int> UsedEntityIds = {};
  std::vector<quads2::QuadData> EntityQuads = {};

  template <typename T>
  int create(T data, int entity_type_id, float camera_type)
  { 
    entity::EntityData edd;
    edd.id = utils2::generate_id(entity::UsedEntityIds);
    edd.texture_id = data.texture_id;
    edd.frame_id = data.current_frame;
    edd.entity_type_id = entity_type_id;
    edd.x = data.x;
    edd.y = data.y;
    edd.h = data.h;
    edd.w = data.w;
    edd.mid_x = edd.x + (edd.w/2);
    edd.mid_y = edd.y + (edd.h/2);
    edd.diag = std::sqrt(std::pow((edd.w/2),2) + std::pow((edd.h/2),2));
    edd.camera_type = camera_type;
    edd.is_solid = true;
    edd.is_clicked = false;
    entity::entities[edd.id] = edd;
    return edd.id;
  }

  int drop(int entity_id)
  { 
    entity::UsedEntityIds.erase(std::remove(entity::UsedEntityIds.begin(), 
                                            entity::UsedEntityIds.end(), entity_id), 
                                            entity::UsedEntityIds.end());
    entity::entities.erase(entity_id);
    return entity_id;
  }

  void render()
  {
    quads2::clear_quads_data(entity::EntityQuads);
    entity::EntityQuads.clear();
    entity::EntityQuads = quads2::make_quads(entity::entities, OBJECT_TYPE_ENTITY);
  }

  void clear()
  {
    entity::UsedEntityIds.clear();
    entity::entities.clear();
    quads2::clear_quads_data(entity::EntityQuads);
    entity::EntityQuads.clear();
  }

  template int entity::create<hero2::HeroData>(hero2::HeroData, int, float);
  template int entity::create<items2::ItemData>(items2::ItemData, int, float);
  template int entity::create<mobs2::MobData>(mobs2::MobData, int, float);

}
