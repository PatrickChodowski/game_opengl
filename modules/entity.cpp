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

    edd.norm_x_start = textures::_get_normalized_frame_x_start(edd.texture_id, edd.frame_id);
    edd.norm_x_end = textures::_get_normalized_frame_x_end(edd.texture_id, edd.frame_id);
    edd.norm_y_start = textures::_get_normalized_frame_y_start(edd.texture_id, edd.frame_id);
    edd.norm_y_end = textures::_get_normalized_frame_y_end(edd.texture_id,edd.frame_id);

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

  
  std::vector<std::string> info(int entity_id)
  {
    // create a string per row
    std::vector<std::string> infos = {};
    entity::EntityData edd = entity::entities[entity_id];
    std::string label_id = "Entity_ID:_" + utils::str(edd.id);
    std::string label_pos = "Pos:_" + utils::str(int(edd.x)) + ',' + utils::str(int(edd.y));
    infos.push_back(label_id);
    infos.push_back(label_pos);
    return infos;
  } 

  void update_frame(int entity_id, int frame_id)
  {
    int texture_id = entity::entities[entity_id].texture_id;
    entity::entities[entity_id].frame_id = frame_id;
    entity::entities[entity_id].norm_x_start = textures::_get_normalized_frame_x_start(texture_id, frame_id);
    entity::entities[entity_id].norm_x_end = textures::_get_normalized_frame_x_end(texture_id, frame_id);
    entity::entities[entity_id].norm_y_start = textures::_get_normalized_frame_y_start(texture_id, frame_id);
    entity::entities[entity_id].norm_y_end = textures::_get_normalized_frame_y_end(texture_id, frame_id);
  }

  template int entity::create<hero::HeroData>(hero::HeroData, int, float);
  template int entity::create<items::ItemData>(items::ItemData, int, float);
  template int entity::create<mobs::MobData>(mobs::MobData, int, float);

}
