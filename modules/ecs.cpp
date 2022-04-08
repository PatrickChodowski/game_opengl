
#include <iostream>
#include <string>
#include <vector>

#include "ecs.h"
#include "utils.h"

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dictionary.h"


namespace ecs
{
  std::vector<int> Index = {};
  phmap::flat_hash_map<unsigned int, ecs::EntityData> entities;

  phmap::flat_hash_map<unsigned int, ecs::PositionComponent> positions;
  phmap::flat_hash_map<unsigned int, ecs::DimensionComponent> dimensions;
  phmap::flat_hash_map<unsigned int, ecs::ModelComponent> models;
  phmap::flat_hash_map<unsigned int, ecs::ColorComponent> colors;

  void init()
  {
    std::cout << "ECS initialized" << std::endl;
  }

  ecs::TempEntityData read_data(std::string& file_name)
  {
    ecs::TempEntityData TD;
    std::string file_path = "./entities/"+file_name+".json";
    std::cout << " [ECS] Reading data from " << file_path << std::endl;
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.allow_missing_members = true;
    context.parseTo(TD);
    return TD;
  }

  //int create_entity(std::string name, std::vector<unsigned int> components)
  int create_entity(ecs::TempEntityData *e)
  {
    int entity_id = utils::generate_id(ecs::Index);
    assert(!ecs::entities.count(entity_id) > 0 && "ERROR DOUBLED ENTITY_ID");

    ecs::EntityData edd;
    edd.entity_id = entity_id;
    edd.name = e -> name;
    edd.components = e -> components;
    ecs::entities.insert(std::pair<int, ecs::EntityData>{edd.entity_id, edd});
    for(int c=0; c<edd.components.size(); c++)
    {
      ecs::add_component(entity_id, edd.components[c], e);
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
    std::vector<unsigned int> components = ecs::entities.at(entity_id).components;

    if(ecs::entities.count(entity_id) > 0)
    {
      ecs::entities.erase(entity_id);
      utils::drop_id(ecs::Index, entity_id);
    }

    for(int c=0; c<components.size(); c++)
    {
      ecs::drop_component(entity_id, components[c]);
    }

  };

  void drop_component(int entity_id, int component_id)
  {

  };

  void add_component(int entity_id, int component_id, ecs::TempEntityData *data)
  { 
    switch(component_id) 
    {
      case COMPONENT_POSITION:
        ecs::_add_position(entity_id, {data->x, data->y, data->z});
      break;
      case COMPONENT_DIMENSION:
        ecs::_add_dimension(entity_id, {data->w, data->h});
      break;
      case COMPONENT_MODEL:
        ecs::_add_model(entity_id, {data->model_id, data->frame_id});
      break;
      case COMPONENT_COLOR:
        ecs::_add_color(entity_id, {data->r, data->g, data->b, data->a});
      break;
    }
  };

  void _add_position(int entity_id, ecs::PositionComponent position)
  {

  };

  void _add_dimension(int entity_id, ecs::DimensionComponent dimension)
  {

  };

  void _add_model(int entity_id, ecs::ModelComponent model)
  {

  };
  
  void _add_color(int entity_id, ecs::ColorComponent color)
  {

  };



}

