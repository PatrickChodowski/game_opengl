
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "anims.h"
#include "ecs.h"
#include "game.h"
#include "models.h"
#include "quads.h"
#include "utils.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"


namespace ecs
{
  std::vector<int> Index = {};
  phmap::flat_hash_map<unsigned int, std::vector<int>> infos;
  std::vector<ecs::TempEntityData> saved_entities;
  phmap::flat_hash_map<unsigned int, ecs::EntityData> entities;
  phmap::flat_hash_map<int, sig_ptr> drop_component;

  phmap::flat_hash_map<unsigned int, ecs::PositionComponent> positions;
  phmap::flat_hash_map<unsigned int, ecs::ModelComponent> models;
  phmap::flat_hash_map<unsigned int, ecs::ColorComponent> colors;
  phmap::flat_hash_map<unsigned int, ecs::RenderdataComponent> renderdatas;
  phmap::flat_hash_map<unsigned int, ecs::ButtonComponent> buttons;
  phmap::flat_hash_map<unsigned int, ecs::LabelComponent> labels;
  // phmap::flat_hash_map<unsigned int, ecs::MoveComponent> moves;
  phmap::flat_hash_map<unsigned int, ecs::StatsComponent> stats;
  phmap::flat_hash_map<unsigned int, ecs::CollisionsComponent> collisions;
  phmap::flat_hash_map<unsigned int, ecs::SensorsComponent> sensors;
  phmap::flat_hash_map<unsigned int, ecs::ItemComponent> items;
  phmap::flat_hash_map<unsigned int, ecs::EquipmentComponent> equipments;
  phmap::flat_hash_map<unsigned int, ecs::NPCComponent> npcs;
  phmap::flat_hash_map<unsigned int, ecs::MobComponent> mobs;
  phmap::flat_hash_map<unsigned int, ecs::DoorComponent> doors;

  void init()
  {
    ecs::drop_component[COMPONENT_POSITION] = ecs::_drop_position;
    ecs::drop_component[COMPONENT_MODEL] = ecs::_drop_model;
    ecs::drop_component[COMPONENT_COLOR] = ecs::_drop_color;
    ecs::drop_component[COMPONENT_RENDERDATA] = ecs::_drop_renderdata;
    ecs::drop_component[COMPONENT_BUTTON] = ecs::_drop_button;
    ecs::drop_component[COMPONENT_LABEL] = ecs::_drop_label;
    // ecs::drop_component[COMPONENT_MOVE] = ecs::_drop_move;
    ecs::drop_component[COMPONENT_STATS] = ecs::_drop_stat;
    ecs::drop_component[COMPONENT_COLLISIONS] = ecs::_drop_collision;
    ecs::drop_component[COMPONENT_SENSORS] = ecs::_drop_sensor;
    ecs::drop_component[COMPONENT_ITEM] = ecs::_drop_item;
    ecs::drop_component[COMPONENT_EQUIPMENT] = ecs::_drop_equipment;
    ecs::drop_component[COMPONENT_NPC] = ecs::_drop_npc;
    ecs::drop_component[COMPONENT_MOB] = ecs::_drop_mob;
    ecs::drop_component[COMPONENT_DOOR] = ecs::_drop_door;
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
    ecs::infos.clear();
  }

  void add_component(int entity_id, int component_id, ecs::TempEntityData *data)
  { 
    switch(component_id) 
    {
      case COMPONENT_POSITION:
        ecs::_add_position(entity_id, {data->x, data->y, data->z, data->w, data->h, data->x, data->y});
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
        ecs::_add_label(entity_id, {data->label, data->text_size, data->text_camera,
                                    data->text_r, data->text_g, data->text_b, data->text_a, 
                                    data->text_x, data->text_y, data->text_z});
      break;
      // case COMPONENT_MOVE:{
      //     // No data to init or pass
      //     float mid_x = data->x + (data->w/2);
      //     float mid_y = data->y + (data->h/2);
      //     ecs::_add_move(entity_id, {data->x, data->y, mid_x, mid_y, 0.0});
      //   break;
      // }
      case COMPONENT_STATS:
        ecs::_add_stat(entity_id, {data->level, data->mobs_killed, 
                                   data->exp, data->speed, data->hp, data->dmg, data->def});
      break;
      case COMPONENT_COLLISIONS:{
          float mid_x = data->x + (data->w/2);
          float mid_y = data->y + (data->h/2);
          phmap::flat_hash_map<int, collisions::AABB> abs;
          ecs::_add_collision(entity_id, {data->radius_x, data->radius_y, mid_x, mid_y, abs, data->is_solid});
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
      case COMPONENT_MOB:
        ecs::_add_mob(entity_id, {data->mob_id});
      break;
      case COMPONENT_DOOR:
        ecs::_add_door(entity_id, {data->door_id, data->dest_scene_id, data->player_enter_x, data->player_enter_y});
      break;
    }
  };

  void _add_position(int entity_id, ecs::PositionComponent position)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding position component for entity " << entity_id << std::endl;
    }
    ecs::positions.insert(std::pair<int, ecs::PositionComponent>{entity_id, position});
  };

  void _add_model(int entity_id, ecs::ModelComponent model)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding model component for entity " << entity_id << std::endl;
    }
    ecs::models.insert(std::pair<int, ecs::ModelComponent>{entity_id, model});
    if(model.model_id > -1){
      models::load(model.model_id);
    }
  };
  
  void _add_color(int entity_id, ecs::ColorComponent color)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding color component for entity " << entity_id << std::endl;
    }
    ecs::colors.insert(std::pair<int, ecs::ColorComponent>{entity_id, color});
  };

  void _add_renderdata(int entity_id, ecs::RenderdataComponent renderdata)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding renderdata component for entity " << entity_id << std::endl;
    }
    ecs::renderdatas.insert(std::pair<int, ecs::RenderdataComponent>{entity_id, renderdata});
  };

  void _add_button(int entity_id, ecs::ButtonComponent button)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding button component for entity " << entity_id << std::endl;
    }
    ecs::buttons.insert(std::pair<int, ecs::ButtonComponent>{entity_id, button});
  };

  void _add_label(int entity_id, ecs::LabelComponent label)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding label component for entity " << entity_id << std::endl;
    }
    ecs::labels.insert(std::pair<int, ecs::LabelComponent>{entity_id, label});
  };

  // void _add_move(int entity_id, ecs::MoveComponent move)
  // {
  //   std::cout << " [ECS] Adding move component for entity " << entity_id << std::endl;
  //   ecs::moves.insert(std::pair<int, ecs::MoveComponent>{entity_id, move});
  // }

  void _add_collision(int entity_id, ecs::CollisionsComponent collision)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding collision component for entity " << entity_id << std::endl;
    }
    ecs::collisions.insert(std::pair<int, ecs::CollisionsComponent>{entity_id, collision});
  }

  void _add_stat(int entity_id, ecs::StatsComponent stat)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding stat component for entity " << entity_id << std::endl;
    }
    ecs::stats.insert(std::pair<int, ecs::StatsComponent>{entity_id, stat});
  }

  void _add_sensor(int entity_id, ecs::SensorsComponent sensor)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding sensors component for entity " << entity_id << std::endl;
    }
    ecs::sensors.insert(std::pair<int, ecs::SensorsComponent>{entity_id, sensor});
  }

  void _add_item(int entity_id, ecs::ItemComponent item)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding item component for entity " << entity_id << std::endl;
    }
    ecs::items.insert(std::pair<int, ecs::ItemComponent>{entity_id, item});
  }

  void _add_equipment(int entity_id, ecs::EquipmentComponent equipment)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding equipment component for entity " << entity_id << std::endl;
    }
    ecs::equipments.insert(std::pair<int, ecs::EquipmentComponent>{entity_id, equipment});
  }

  void _add_npc(int entity_id, ecs::NPCComponent npc)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding npc component for entity " << entity_id << std::endl;
    }
    ecs::npcs.insert(std::pair<int, ecs::NPCComponent>{entity_id, npc});
  }

  void _add_mob(int entity_id, ecs::MobComponent mob)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding mob component for entity " << entity_id << std::endl;
    }
    ecs::mobs.insert(std::pair<int, ecs::MobComponent>{entity_id, mob});
  }

  void _add_door(int entity_id, ecs::DoorComponent door)
  {
    if(game::IS_DEBUG_MODE){
      std::cout << " [ECS] Adding door component for entity " << entity_id << std::endl;
    }
    ecs::doors.insert(std::pair<int, ecs::DoorComponent>{entity_id, door});
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

  // void _drop_move(int entity_id)
  // {
  //   std::cout << " [ECS] Dropping move component for entity " << entity_id << std::endl;
  //   ecs::moves.erase(entity_id);
  // };

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

  void _drop_mob(int entity_id)
  {
    std::cout << " [ECS] Dropping mob component for entity " << entity_id << std::endl;
    ecs::mobs.erase(entity_id);
  };

  void _drop_door(int entity_id)
  {
    std::cout << " [ECS] Dropping door component for entity " << entity_id << std::endl;
    ecs::doors.erase(entity_id);
  };



  void update_position_diff(int entity_id, float x, float y)
  {
    if(ecs::positions.count(entity_id))
    { 
      ecs::PositionComponent pos = ecs::positions.at(entity_id);

      float new_x = pos.x + x;
      float new_y = pos.y - y;
      ecs::positions.at(entity_id).prev_x = pos.x;
      ecs::positions.at(entity_id).prev_y = pos.y;
      ecs::positions.at(entity_id).x = new_x;
      ecs::positions.at(entity_id).y = new_y;

      if(ecs::collisions.count(entity_id))
      {
        ecs::collisions.at(entity_id).mid_x = new_x + (pos.w/2);
        ecs::collisions.at(entity_id).mid_y = new_y + (pos.h/2);
      }

      //std::cout << " [ECS][MOVE] Updating position for entity " << entity_id << " new pos: (" << new_x << "," << new_y << ")"<< std::endl;
    }
  }

  void update_position(int entity_id, float x, float y)
  {
    if(ecs::positions.count(entity_id))
    { 
      ecs::PositionComponent pos = ecs::positions.at(entity_id);
      float new_x = x;
      float new_y = y;
      ecs::positions.at(entity_id).prev_x = pos.x;
      ecs::positions.at(entity_id).prev_y = pos.y;
      ecs::positions.at(entity_id).x = new_x;
      ecs::positions.at(entity_id).y = new_y;

      if(ecs::collisions.count(entity_id))
      {
        ecs::collisions.at(entity_id).mid_x = new_x + (pos.w/2);
        ecs::collisions.at(entity_id).mid_y = new_y + (pos.h/2);
      }

      //std::cout << " [ECS][MOVE] Updating position for entity " << entity_id << " new pos: (" << new_x << "," << new_y << ")"<< std::endl;
    }
  }

  void set_position(int entity_id, float x, float y)
  {
    if(ecs::positions.count(entity_id))
    {
      ecs::positions.at(entity_id).x = x;
      ecs::positions.at(entity_id).y = y;
      ecs::positions.at(entity_id).prev_y = y;
      ecs::positions.at(entity_id).prev_x = x;

      if(ecs::collisions.count(entity_id))
      {
        ecs::collisions.at(entity_id).mid_x = x + (ecs::positions.at(entity_id).w/2);
        ecs::collisions.at(entity_id).mid_y = y + (ecs::positions.at(entity_id).h/2);
      }

      std::cout << " [ECS][POSITION] Setting position of entity " << entity_id << " to (" << x << "," << y << ")" << std::endl;
    } 
  }

  void revert_position_x(int entity_id)
  {
    ecs::positions.at(entity_id).x = ecs::positions.at(entity_id).prev_x;
    //std::cout << " [ECS][POSITION] Reverting X position to " << ecs::positions.at(entity_id).prev_x << " from " << ecs::positions.at(entity_id).x << std::endl;
  }

  void revert_position_y(int entity_id)
  {
    ecs::positions.at(entity_id).y = ecs::positions.at(entity_id).prev_y;
    //std::cout << " [ECS][POSITION] Reverting Y position to " << ecs::positions.at(entity_id).prev_y << " from " << ecs::positions.at(entity_id).y << std::endl;
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

  void save_temp(int entity_id)
  {
    ecs::saved_entities.clear();

    std::cout << " [ECS] Saving entity_id " << entity_id << std::endl;
    ecs::TempEntityData e;
    e.entity_type_id = ecs::entities.at(entity_id).entity_type_id;
    e.name = ecs::entities.at(entity_id).name;
    e.components = ecs::entities.at(entity_id).components;

    for(int c=0; c<e.components.size();c++)
    {
      int component_id = e.components[c];
      switch(component_id)
      {
        case COMPONENT_POSITION:{
          ecs::PositionComponent pos = ecs::positions.at(entity_id);
          e.x = pos.x;
          e.y = pos.y;
          e.z = pos.z;
          e.w = pos.w;
          e.h = pos.h;
        }
        break;
        case COMPONENT_MODEL:{
          ecs::ModelComponent mod = ecs::models.at(entity_id);
          e.model_id = mod.model_id;
          e.frame_id = mod.frame_id;
          e.side_id = mod.side_id;
        }
        break;
        case COMPONENT_COLOR:{
          ecs::ColorComponent col = ecs::colors.at(entity_id);
          e.r = col.r;
          e.g = col.g;
          e.b = col.b;
          e.a = col.a;
        }
        break;
        case COMPONENT_RENDERDATA:
          e.camera_type = ecs::renderdatas.at(entity_id).camera_type;
        break;
        case COMPONENT_BUTTON:
          e.button_function_id = ecs::buttons.at(entity_id).button_function_id;
        break;
        case COMPONENT_LABEL:{
          ecs::LabelComponent lab = ecs::labels.at(entity_id);
          e.label = lab.label;
          e.text_size = lab.text_size;
          e.text_camera = lab.text_camera;
          e.text_r = lab.text_r;
          e.text_g = lab.text_g;
          e.text_b = lab.text_b;
          e.text_a = lab.text_a;
          e.text_x = lab.text_x;
          e.text_y = lab.text_y;
          e.text_z = lab.text_z;
        }
        break;
        case COMPONENT_STATS:{
          ecs::StatsComponent stat = ecs::stats.at(entity_id);
          e.level = stat.level;
          e.mobs_killed = stat.mobs_killed;
          e.exp = stat.exp;
          e.speed = stat.speed;
          e.hp = stat.hp;
          e.dmg = stat.dmg;
          e.def = stat.def;
        }
        break;
        case COMPONENT_COLLISIONS:{
            ecs::CollisionsComponent coll = ecs::collisions.at(entity_id);
            e.radius_x = coll.radius_x;
            e.radius_y = coll.radius_y;
            e.is_solid = coll.is_solid;
        }
          break;
        case COMPONENT_SENSORS:
          // 
        break;
        case COMPONENT_ITEM:{
          ecs::ItemComponent item = ecs::items.at(entity_id);
          e.item_id = item.item_id;
          e.item_joint_id = item.item_joint_id;
          e.item_dmg = item.item_dmg;
          e.item_speed = item.item_speed;
          e.item_location = item.item_location;
        }
        break;
        case COMPONENT_EQUIPMENT:
          e.equipment = ecs::equipments.at(entity_id).equipment;
        break;
        case COMPONENT_NPC:{
          ecs::NPCComponent npc = ecs::npcs.at(entity_id);
          e.npc_id = npc.npc_id;
          e.personality_trait_id = npc.npc_id;
          e.sentiment = npc.sentiment;
        }
        break;
        case COMPONENT_MOB:
          e.mob_id = ecs::mobs.at(entity_id).mob_id;
        break;
        case COMPONENT_DOOR:{
          ecs::DoorComponent door = ecs::doors.at(entity_id);
          e.door_id = door.door_id;
          e.dest_scene_id = door.dest_scene_id;
          e.player_enter_x = door.player_enter_x;
          e.player_enter_y = door.player_enter_y;
        }
        break;
      }


    }

    ecs::saved_entities.push_back(e);
  }

  void info(int entity_id)
  {
    ecs::infos.clear();
    std::vector<int> info_entities = {};
    std::vector<std::string> infos = {};

    ecs::EntityData ent = ecs::entities.at(entity_id);
    infos.push_back(ent.name + "(" + std::to_string(entity_id) + ") type: " + std::to_string(ent.entity_type_id)); 

    if(ecs::positions.count(entity_id)){
      ecs::PositionComponent pos = ecs::positions.at(entity_id);
      infos.push_back("pos: " + utils::str(int(pos.x))+ ", " + utils::str(int(pos.y)));
      infos.push_back("dim: " + utils::str(int(pos.w))+ ", " + utils::str(int(pos.h)));
    }

    if(ecs::stats.count(entity_id)){
      ecs::StatsComponent stat = ecs::stats.at(entity_id);
      infos.push_back("level: " + utils::str(stat.level));
      infos.push_back("hp: " + utils::str(int(stat.hp)));
      infos.push_back("dmg: " + utils::str(int(stat.dmg)));
      infos.push_back("def: " + utils::str(int(stat.def)));
      infos.push_back("exp: " + utils::str(int(stat.exp)));
    }

    if(ecs::items.count(entity_id)){
      ecs::ItemComponent item = ecs::items.at(entity_id);
      infos.push_back("item_id: " + utils::str(item.item_id));
    }

    if(ecs::mobs.count(entity_id)){
      ecs::MobComponent mob = ecs::mobs.at(entity_id);
      infos.push_back("mob_id: " + utils::str(mob.mob_id));
    }

    if(ecs::npcs.count(entity_id)){
      ecs::NPCComponent npc = ecs::npcs.at(entity_id);
      infos.push_back("npc_id: " + utils::str(npc.npc_id));
      infos.push_back("sentiment: " + utils::str(npc.sentiment));
    }
    // menu background:

      ecs::TempEntityData e0;
      e0.name = "info_menu_background_" + std::to_string(entity_id);
      e0.components = {0,1,2,3};
      e0.entity_type_id = ENTITY_TYPE_COLOR;
      e0.x = 600;
      e0.y = 5;
      e0.z = 0.9;
      e0.w = 350;
      e0.h = 500;
      e0.r = 0.701,
      e0.g = 0.675,
      e0.b = 0.675,
      e0.a = 0.6;
      e0.model_id = -1;
      e0.frame_id = -1;
      e0.side_id = 0;
      e0.camera_type = CAMERA_STATIC;
      info_entities.push_back(ecs::create_entity(&e0));


    float start_y = 10;
    for(int s=0; s<infos.size(); s++)
    {
      // Info menu definition
      ecs::TempEntityData e;
      e.name = "info_menu_" + std::to_string(entity_id);
      e.components = {5};
      e.entity_type_id = ENTITY_TYPE_TEXT;
      e.label = infos[s];
      e.text_camera = CAMERA_STATIC;
      e.text_size = 25;
      e.text_r = 0.8;
      e.text_g = 0.8;
      e.text_b = 0.8;
      e.text_a = 1.0;
      e.text_x = 605;
      e.text_y = start_y;
      e.text_z = 0.99;
      info_entities.push_back(ecs::create_entity(&e));
      start_y += 30;
    }
    ecs::infos.insert(std::pair<unsigned int, std::vector<int>>{entity_id, info_entities});
  }

  void clear_info()
  {
    for (auto const& [entity_id, info_ids] : ecs::infos)
    {
      for(int i=0; i<info_ids.size(); i++)
      {
        ecs::drop(info_ids[i]);
      }
    }
    ecs::infos.clear();
  }
}

