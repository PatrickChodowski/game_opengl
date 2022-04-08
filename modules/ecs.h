

#include <string>
#include <vector>

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"

#ifndef MODULES_ECS_H
#define MODULES_ECS_H

namespace ecs 
{
  // Entity IDs Index
  extern std::vector<int> Index;

  // Entity Data consists of entity id and bitmap of components
  struct EntityData
  {
    std::string name;
    unsigned int entity_id;
    std::vector<unsigned int> components;
  };

  // Contains all possible entity attributes, used for loading the entity data
  struct TempEntityData
  { 
    // Entity data (Required)
    std::string name;
    std::vector<unsigned int> components;

    // Optionals (although there is no use for entity without 
    // position, dimension or one of model/color)

    // Position
    float x,y,z;
    // Dimension
    float w,h;
    // Model
    int model_id, frame_id;
    // Color
    float r,g,b,a;

    // All attributes can be read from JSON
    JS_OBJ(name, components, 
           x,y,z,
           w,h,
           model_id,frame_id,
           r,g,b,a);
  };


  // Component 0: Position data -> x, y, z
  struct PositionComponent
  { 
    float x;
    float y;
    float z;
  };

  // Component 1: Dimension data -> w, h
  struct DimensionComponent
  { 
    float w;
    float h;
  };

  // Component 2: Model data -> model_id, frame_id
  struct ModelComponent
  { 
    int model_id;
    int frame_id;
  };

  // Component 3: Color data -> r,g,b,a
  struct ColorComponent
  { 
    float r;
    float g;
    float b;
    float a;
  };


  // TABLES
  extern phmap::flat_hash_map<unsigned int, ecs::EntityData> entities;
  extern phmap::flat_hash_map<unsigned int, ecs::PositionComponent> positions;
  extern phmap::flat_hash_map<unsigned int, ecs::DimensionComponent> dimensions;
  extern phmap::flat_hash_map<unsigned int, ecs::ModelComponent> models;
  extern phmap::flat_hash_map<unsigned int, ecs::ColorComponent> colors;




  // METHODS

  // Initializes component system
  void init();

  // Read entity data and return pointer to the struct with data
  ecs::TempEntityData read_data(std::string& file_name);

  // Creates entity and propagates components based on the TempEntityData struct.
  //  Usage, option 1:   ecs::TempEntityData entity; ecs::create_entity(&entity);
  //  Usage, option 2: ecs::create_entity(&entity);
  int create_entity(ecs::TempEntityData *e);

  // Wrapper around ecs::read_data and ecs::create_entity
  int create_entity_from_file(std::string file_name);

  // Definately not the priettest solution
  void add_component(int entity_id, int component_id, ecs::TempEntityData *data);

  // Component adding methods
  void _add_position(int entity_id, ecs::PositionComponent position);
  void _add_dimension(int entity_id, ecs::DimensionComponent dimension);
  void _add_model(int entity_id, ecs::ModelComponent model);
  void _add_color(int entity_id, ecs::ColorComponent color);
}


#endif
