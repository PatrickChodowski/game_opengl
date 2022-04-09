

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

  typedef void (*sig_ptr)(int entity_id);
  // Drop component functor
  extern phmap::flat_hash_map<int, sig_ptr> drop_component;

  // Entity Data consists of entity id and bitmap of components
  struct EntityData
  {
    std::string name;
    unsigned int entity_id;
    int entity_type_id;
    std::vector<unsigned int> components;
  };

  // Contains all possible entity attributes, used for loading the entity data
  struct TempEntityData
  { 
    // Entity data (Required)
    std::string name;
    std::string label; // LabelComponent
    std::vector<unsigned int> components;
    int entity_type_id;

    // Optionals (although there is no use for entity without 
    // position, dimension or one of model/color)

    // Position
    float x,y,z;
    // Dimension
    float w,h;
    // Model
    int model_id, frame_id, side_id;
    // Color
    float r,g,b,a;
    // Render data
    int camera_type;
    // Button
    int button_function_id;
    // Label
    float text_size;
    float text_r, text_g, text_b, text_a;
    float text_x, text_y, text_z;

    // All attributes can be read from JSON
    JS_OBJ(name, components, entity_type_id,
           x,y,z,
           w,h,
           model_id,frame_id,side_id,
           r,g,b,a,
           camera_type,
           button_function_id,
           label, text_size, text_r, text_g, text_b, text_a, text_x, text_y, text_z);
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

  // Component 2: Model data -> model_id, frame_id, side_id
  struct ModelComponent
  { 
    int model_id;
    int frame_id;
    int side_id;
  };

  // Component 3: Color data -> r,g,b,a
  struct ColorComponent
  { 
    float r;
    float g;
    float b;
    float a;
  };

  // Component 4: Render data. Everything else needed for correct rendering
  struct RenderdataComponent
  {
    int camera_type;
    bool is_clicked = false;
    bool visible = true;
  };

  // Component 5: Button component
  struct ButtonComponent
  {
    int button_function_id;
  };

  // Component 6: Label component
  struct LabelComponent
  {
    std::string label;
    float text_size;
    float text_r;
    float text_g; 
    float text_b; 
    float text_a;
    float text_x; 
    float text_y;
    float text_z;
  };


  // TABLES
  extern phmap::flat_hash_map<unsigned int, ecs::EntityData> entities;
  extern phmap::flat_hash_map<unsigned int, ecs::PositionComponent> positions;
  extern phmap::flat_hash_map<unsigned int, ecs::DimensionComponent> dimensions;
  extern phmap::flat_hash_map<unsigned int, ecs::ModelComponent> models;
  extern phmap::flat_hash_map<unsigned int, ecs::ColorComponent> colors;
  extern phmap::flat_hash_map<unsigned int, ecs::RenderdataComponent> renderdatas;
  extern phmap::flat_hash_map<unsigned int, ecs::ButtonComponent> buttons;
  extern phmap::flat_hash_map<unsigned int, ecs::LabelComponent> labels;




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

  // Drop entity
  void drop(int entity_id);

  // Temporarily here? Renders entity from ecs
  void render();

  // Clears entity data
  void clear();

  // Definately not the priettest solution
  void add_component(int entity_id, int component_id, ecs::TempEntityData *data);

  // Component adding methods
  void _add_position(int entity_id, ecs::PositionComponent position);
  void _add_dimension(int entity_id, ecs::DimensionComponent dimension);
  void _add_model(int entity_id, ecs::ModelComponent model);
  void _add_color(int entity_id, ecs::ColorComponent color);
  void _add_renderdata(int entity_id, ecs::RenderdataComponent renderdata);
  void _add_button(int entity_id, ecs::ButtonComponent button);
  void _add_label(int entity_id, ecs::LabelComponent labels);


  // Component dropping methods
  void _drop_position(int entity_id);
  void _drop_dimension(int entity_id);
  void _drop_model(int entity_id);
  void _drop_color(int entity_id);
  void _drop_renderdata(int entity_id);
  void _drop_button(int entity_id);
  void _drop_label(int entity_id);

}


#endif
