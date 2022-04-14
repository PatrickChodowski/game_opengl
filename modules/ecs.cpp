
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "anims.h"
#include "ecs.h"
#include "models.h"
#include "quads.h"
#include "utils.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"


namespace ecs
{
  std::vector<int> Index = {};
  phmap::flat_hash_map<unsigned int, ecs::EntityData> entities;
  phmap::flat_hash_map<int, sig_ptr> drop_component;

  phmap::flat_hash_map<unsigned int, ecs::PositionComponent> positions;
  phmap::flat_hash_map<unsigned int, ecs::ModelComponent> models;
  phmap::flat_hash_map<unsigned int, ecs::ColorComponent> colors;
  phmap::flat_hash_map<unsigned int, ecs::RenderdataComponent> renderdatas;
  phmap::flat_hash_map<unsigned int, ecs::ButtonComponent> buttons;
  phmap::flat_hash_map<unsigned int, ecs::LabelComponent> labels;
  phmap::flat_hash_map<unsigned int, ecs::MoveComponent> moves;
  phmap::flat_hash_map<unsigned int, ecs::StatsComponent> stats;
  phmap::flat_hash_map<unsigned int, ecs::CollisionsComponent> collisions;
  phmap::flat_hash_map<unsigned int, ecs::SensorsComponent> sensors;
  phmap::flat_hash_map<unsigned int, ecs::ItemComponent> items;
  phmap::flat_hash_map<unsigned int, ecs::EquipmentComponent> equipments;
  phmap::flat_hash_map<unsigned int, ecs::NPCComponent> npcs;

  void init()
  {
    ecs::drop_component[COMPONENT_POSITION] = ecs::_drop_position;
    ecs::drop_component[COMPONENT_MODEL] = ecs::_drop_model;
    ecs::drop_component[COMPONENT_COLOR] = ecs::_drop_color;
    ecs::drop_component[COMPONENT_RENDERDATA] = ecs::_drop_renderdata;
    ecs::drop_component[COMPONENT_BUTTON] = ecs::_drop_button;
    ecs::drop_component[COMPONENT_LABEL] = ecs::_drop_label;
    ecs::drop_component[COMPONENT_MOVE] = ecs::_drop_move;
    ecs::drop_component[COMPONENT_STATS] = ecs::_drop_stat;
    ecs::drop_component[COMPONENT_COLLISIONS] = ecs::_drop_collision;
    ecs::drop_component[COMPONENT_SENSORS] = ecs::_drop_sensor;
    ecs::drop_component[COMPONENT_ITEM] = ecs::_drop_item;
    ecs::drop_component[COMPONENT_EQUIPMENT] = ecs::_drop_equipment;
    ecs::drop_component[COMPONENT_NPC] = ecs::_drop_npc;
    std::cout << "ECS initialized" << std::endl;
  }

  ecs::TempEntityData read_data(std::string& file_name)
  {
    ecs::TempEntityData TD;
    std::string file_path = "./data/entities/"+file_name+".json";
    std::cout << " [ECS] Reading data from " << file_path << std::endl;
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.allow_missing_members = true;
    context.parseTo(TD);
    return TD;
  }

  int create_entity(ecs::TempEntityData *e)
  {
    int entity_id = utils::generate_id(ecs::Index);
    assert(!(ecs::entities.count(entity_id) > 0) && "ERROR DOUBLED ENTITY_ID");

    ecs::EntityData edd;
    edd.entity_id = entity_id;
    edd.name = e -> name;
    edd.components = e -> components;
    edd.entity_type_id = e -> entity_type_id;

    ecs::entities.insert(std::pair<int, ecs::EntityData>{edd.entity_id, edd});
    for(int c=0; c<edd.components.size(); c++)
    {
      ecs::add_component(entity_id, edd.components[c], e);
    }

    if(e->animated){
      anims::start(ANIM_STANDING_IDLE, entity_id);
    }

    std::cout << " [ECS] Created entity name " << edd.name << " ID: " << entity_id << std::endl;     
    return entity_id;
  }

  int create_entity_from_file(std::string file_name)
  {
    ecs::TempEntityData e = ecs::read_data(file_name);
    int entity_id = ecs::create_entity(&e);
    return entity_id;
  }

  void drop(int entity_id)
  {
    std::cout << " [ECS] Dropping entity " << entity_id << std::endl;
    std::vector<unsigned int> components = ecs::entities.at(entity_id).components;

    if(ecs::entities.count(entity_id) > 0){
      ecs::entities.erase(entity_id);
      utils::drop_id(ecs::Index, entity_id);
    }

    for(int c=0; c<components.size(); c++)
    {
      ecs::drop_component.at(components[c])(entity_id);
    }
    
  };

  void clear()
  {
    for(auto const& [entity_id, entity_data]: ecs::entities)
    {
      ecs::drop(entity_id);
    }
    ecs::entities.clear();
  }

  void add_component(int entity_id, int component_id, ecs::TempEntityData *data)
  { 
    switch(component_id) 
    {
      case COMPONENT_POSITION:
        ecs::_add_position(entity_id, {data->x, data->y, data->z, data->w, data->h});
      break;
      case COMPONENT_MODEL:
        ecs::_add_model(entity_id, {data->model_id, data->frame_id, data->side_id});
      break;
      case COMPONENT_COLOR:
        ecs::_add_color(entity_id, {data->r, data->g, data->b, data->a});
      break;
      case COMPONENT_RENDERDATA:
        ecs::_add_renderdata(entity_id, {data->camera_type});
      break;
      case COMPONENT_BUTTON:
        ecs::_add_button(entity_id, {data->button_function_id});
      break;
      case COMPONENT_LABEL:
        ecs::_add_label(entity_id, {data->label, data->text_size, 
                                    data->text_r, data->text_g, data->text_b, data->text_a, 
                                    data->text_x, data->text_y, data->text_z});
      break;
      case COMPONENT_MOVE:{
          // No data to init or pass
          float mid_x = data->x + (data->w/2);
          float mid_y = data->y + (data->h/2);
          ecs::_add_move(entity_id, {data->x, data->y, mid_x, mid_y, 0.0});
        break;
      }
      case COMPONENT_STATS:
        ecs::_add_stat(entity_id, {data->level, data->mobs_killed, 
                                   data->exp, data->speed, data->hp, data->dmg, data->def});
      break;
      case COMPONENT_COLLISIONS:{
          float diag = std::sqrt(std::pow((data->w/2),2) + std::pow((data->h/2),2));
          phmap::flat_hash_map<int, collisions::AABB> abs;
          ecs::_add_collision(entity_id, {diag, abs, data->is_solid});
        break;
      }
      case COMPONENT_SENSORS:
        ecs::_add_sensor(entity_id, {});
      break;
      case COMPONENT_ITEM:
        ecs::_add_item(entity_id, {data->item_id, data->item_joint_id, 
                                   data->item_dmg, data->item_speed, data->item_location});
      break;
      case COMPONENT_EQUIPMENT:
        ecs::_add_equipment(entity_id, {data->equipment});
      break;
      case COMPONENT_NPC:
        ecs::_add_npc(entity_id, {data->npc_id, data->personality_trait_id, data->sentiment});
      break;
    }
  };

  void _add_position(int entity_id, ecs::PositionComponent position)
  {
    std::cout << " [ECS] Adding position component for entity " << entity_id << std::endl;
    ecs::positions.insert(std::pair<int, ecs::PositionComponent>{entity_id, position});
  };

  void _add_model(int entity_id, ecs::ModelComponent model)
  {
    std::cout << " [ECS] Adding model component for entity " << entity_id << std::endl;
    ecs::models.insert(std::pair<int, ecs::ModelComponent>{entity_id, model});
    if(model.model_id > -1){
      models::load(model.model_id);
    }
  };
  
  void _add_color(int entity_id, ecs::ColorComponent color)
  {
    std::cout << " [ECS] Adding color component for entity " << entity_id << std::endl;
    ecs::colors.insert(std::pair<int, ecs::ColorComponent>{entity_id, color});
  };

  void _add_renderdata(int entity_id, ecs::RenderdataComponent renderdata)
  {
    std::cout << " [ECS] Adding renderdata component for entity " << entity_id << std::endl;
    ecs::renderdatas.insert(std::pair<int, ecs::RenderdataComponent>{entity_id, renderdata});
  };

  void _add_button(int entity_id, ecs::ButtonComponent button)
  {
    std::cout << " [ECS] Adding button component for entity " << entity_id << std::endl;
    ecs::buttons.insert(std::pair<int, ecs::ButtonComponent>{entity_id, button});
  };

  void _add_label(int entity_id, ecs::LabelComponent label)
  {
    std::cout << " [ECS] Adding label component for entity " << entity_id << std::endl;
    ecs::labels.insert(std::pair<int, ecs::LabelComponent>{entity_id, label});
  };

  void _add_move(int entity_id, ecs::MoveComponent move)
  {
    std::cout << " [ECS] Adding move component for entity " << entity_id << std::endl;
    ecs::moves.insert(std::pair<int, ecs::MoveComponent>{entity_id, move});
  }

  void _add_collision(int entity_id, ecs::CollisionsComponent collision)
  {
    std::cout << " [ECS] Adding collision component for entity " << entity_id << std::endl;
    ecs::collisions.insert(std::pair<int, ecs::CollisionsComponent>{entity_id, collision});
  }

  void _add_stat(int entity_id, ecs::StatsComponent stat)
  {
    std::cout << " [ECS] Adding stat component for entity " << entity_id << std::endl;
    ecs::stats.insert(std::pair<int, ecs::StatsComponent>{entity_id, stat});
  }

  void _add_sensor(int entity_id, ecs::SensorsComponent sensor)
  {
    std::cout << " [ECS] Adding sensors component for entity " << entity_id << std::endl;
    ecs::sensors.insert(std::pair<int, ecs::SensorsComponent>{entity_id, sensor});
  }

  void _add_item(int entity_id, ecs::ItemComponent item)
  {
    std::cout << " [ECS] Adding item component for entity " << entity_id << std::endl;
    ecs::items.insert(std::pair<int, ecs::ItemComponent>{entity_id, item});
  }

  void _add_equipment(int entity_id, ecs::EquipmentComponent equipment)
  {
    std::cout << " [ECS] Adding equipment component for entity " << entity_id << std::endl;
    ecs::equipments.insert(std::pair<int, ecs::EquipmentComponent>{entity_id, equipment});
  }

  void _add_npc(int entity_id, ecs::NPCComponent npc)
  {
    std::cout << " [ECS] Adding npc component for entity " << entity_id << std::endl;
    ecs::npcs.insert(std::pair<int, ecs::NPCComponent>{entity_id, npc});
  }



  void _drop_position(int entity_id)
  {
    std::cout << " [ECS] Dropping position component for entity " << entity_id << std::endl;
    ecs::positions.erase(entity_id);
  };

  void _drop_model(int entity_id)
  {
    std::cout << " [ECS] Dropping model component for entity " << entity_id << std::endl;
    ecs::models.erase(entity_id);
  };

  void _drop_color(int entity_id)
  {
    std::cout << " [ECS] Dropping color component for entity " << entity_id << std::endl;
    ecs::colors.erase(entity_id);
  };

  void _drop_renderdata(int entity_id)
  {
    std::cout << " [ECS] Dropping renderdata component for entity " << entity_id << std::endl;
    ecs::renderdatas.erase(entity_id);
  };

  void _drop_button(int entity_id)
  {
    std::cout << " [ECS] Dropping button component for entity " << entity_id << std::endl;
    ecs::buttons.erase(entity_id);
  };

  void _drop_label(int entity_id)
  {
    std::cout << " [ECS] Dropping label component for entity " << entity_id << std::endl;
    ecs::labels.erase(entity_id);
  };

  void _drop_move(int entity_id)
  {
    std::cout << " [ECS] Dropping move component for entity " << entity_id << std::endl;
    ecs::moves.erase(entity_id);
  };

  void _drop_stat(int entity_id)
  {
    std::cout << " [ECS] Dropping stat component for entity " << entity_id << std::endl;
    ecs::stats.erase(entity_id);
  };

  void _drop_collision(int entity_id)
  {
    std::cout << " [ECS] Dropping collisions component for entity " << entity_id << std::endl;
    ecs::collisions.erase(entity_id);
  };

  void _drop_sensor(int entity_id)
  {
    std::cout << " [ECS] Dropping sensors component for entity " << entity_id << std::endl;
    ecs::sensors.erase(entity_id);
  };

  void _drop_item(int entity_id)
  {
    std::cout << " [ECS] Dropping item component for entity " << entity_id << std::endl;
    ecs::items.erase(entity_id);
  };

  void _drop_equipment(int entity_id)
  {
    std::cout << " [ECS] Dropping equipment component for entity " << entity_id << std::endl;
    ecs::equipments.erase(entity_id);
  };

  void _drop_npc(int entity_id)
  {
    std::cout << " [ECS] Dropping npc component for entity " << entity_id << std::endl;
    ecs::npcs.erase(entity_id);
  };



  void update_position(int entity_id, float x, float y)
  {
    if(ecs::moves.count(entity_id))
    { 
      float new_x = ecs::positions.at(entity_id).x += x;
      float new_y = ecs::positions.at(entity_id).y -= y;
      ecs::moves.at(entity_id).prev_x = ecs::positions.at(entity_id).x;
      ecs::moves.at(entity_id).prev_y = ecs::positions.at(entity_id).y;
      ecs::moves.at(entity_id).mid_x = new_x + (ecs::positions.at(entity_id).w/2);
      ecs::moves.at(entity_id).mid_y = new_y + (ecs::positions.at(entity_id).h/2);
      ecs::positions.at(entity_id).x = new_x;
      ecs::positions.at(entity_id).y = new_y;

      //std::cout << " [ECS][MOVE] Updating position for entity " << entity_id << " new pos: (" << new_x << "," << new_y << ")"<< std::endl;
    }
  }

  void set_position(int entity_id, float x, float y)
  {
    if(ecs::positions.count(entity_id))
    {
      ecs::positions.at(entity_id).x = x;
      ecs::positions.at(entity_id).y = y;
      std::cout << " [ECS][POSITION] Setting position of entity " << entity_id << " to (" << x << "," << y << ")" << std::endl;
    } 
  }

  void revert_position_x(int entity_id)
  {
    ecs::positions.at(entity_id).x = ecs::moves.at(entity_id).prev_x;
    //hero::_update_joints();
  }

  void revert_position_y(int entity_id)
  {
    ecs::positions.at(entity_id).y = ecs::moves.at(entity_id).prev_y;
    //hero::_update_joints();
  }

  void hide(int entity_id)
  {
    if(ecs::renderdatas.count(entity_id)){
      ecs::renderdatas.at(entity_id).camera_type = CAMERA_HIDDEN;
    }
  };

  void show(int entity_id)
  {
    if(ecs::renderdatas.count(entity_id)){
      ecs::renderdatas.at(entity_id).camera_type = CAMERA_DYNAMIC;
    }
  };


}

