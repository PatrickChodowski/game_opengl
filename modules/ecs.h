

#include <string>
#include <vector>

#include "collisions.h"

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
    float x,y,z,w,h;
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


    // Stats (for hero, for mobs etc.)
    int level, mobs_killed;
    float exp, speed, hp, dmg, def;
    
    
    // Collisions
    float radius_x, radius_y;
    bool is_solid = true;

    // Sensors
    // Nothing to add here

    // Items
    int item_id, item_joint_id, item_dmg, item_speed, item_location;

    // Equipment
    std::vector<int> equipment;

    // NPC
    int npc_id;
    int personality_trait_id;
    float sentiment;

    // Mob
    int mob_id;

    // Door
    int door_id, dest_scene_id;
    float player_enter_x, player_enter_y;


    // Additional (not stored in any container)
    bool animated = false;


    // All attributes can be read from JSON
    JS_OBJ(name, components, entity_type_id,
           x,y,z,w,h,
           model_id,frame_id,side_id,
           r,g,b,a,
           camera_type,
           button_function_id,
           label, text_size, text_r, text_g, text_b, text_a, text_x, text_y, text_z,
           level, mobs_killed, exp, speed, hp, dmg, def,
           radius_x, radius_y, is_solid,
           item_id, item_joint_id, item_dmg, item_speed, item_location,
           equipment,
           npc_id, personality_trait_id, sentiment,
           mob_id,
           door_id, dest_scene_id, player_enter_x, player_enter_y,
           animated);
  };

  // Temporarily saved entity data
  extern std::vector<ecs::TempEntityData> saved_entities;

  // Component 0: Position data -> x, y, z, w, h, prev_x, prev_y
  struct PositionComponent
  { 
    float x;
    float y;
    float z;
    float w;
    float h;
    float prev_x;
    float prev_y;
  };

  // Component 1: Model data -> model_id, frame_id, side_id
  struct ModelComponent
  { 
    int model_id;
    int frame_id;
    int side_id;
  };

  // Component 2: Color data -> r,g,b,a
  struct ColorComponent
  { 
    float r;
    float g;
    float b;
    float a;
  };

  // Component 3: Render data. Everything else needed for correct rendering
  struct RenderdataComponent
  {
    int camera_type;
    bool is_clicked = false;
  };

  // Component 4: Button component -> button_function_id
  struct ButtonComponent
  {
    int button_function_id;
  };

  // Component 5: Label component -> label, size, r,g,b,a,x,y,z
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

  // // Component 6: Move component -> prev_x, prev_y, mid_x, mid_y, direction
  // struct MoveComponent
  // {
  //   float prev_x;
  //   float prev_y;
  //   float mid_x;
  //   float mid_y;
  //   float direction;
  // };

  // Component 7: Stats component -> level, mobs_killed, exp, speed, hp, dmg, def
  struct StatsComponent
  {
    int level;
    int mobs_killed;
    float exp;
    float speed;
    float hp;
    float dmg;
    float def;
  };

  // Component 8: Collisions component -> radius, mid, point, AABB boxes, is_solid
  struct CollisionsComponent
  {
    float radius_x;
    float radius_y;
    float mid_x;
    float mid_y;
    phmap::flat_hash_map<int, collisions::AABB> abs; 
    bool is_solid;
  };

  // Component 9: Sensors component (for collisions) -> sensors vectors
  struct SensorsComponent
  {
    phmap::flat_hash_map<int, collisions::Sensor> sensors; 
  };


  // Component 10: Item component -> item attributes if given entity is an item.
  // Used for generated items (on ground and in hand)
  struct ItemComponent
  {
    int item_id;
    int item_joint_id;
    int item_dmg;
    int item_speed;
    int item_location;
  };
  //ITEM_LOCATION_GROUND 0,ITEM_LOCATION_EQ 1

  // Component 11: Equipment component -> List of items (entities) equipped by entity(hero)
  struct EquipmentComponent
  {
    std::vector<int> equipment;
  };

  // Component 12: NPC component -> npc interactions data
  struct NPCComponent
  {
    int npc_id;
    int personality_trait_id;
    float sentiment;
  };

  // Component 13: MOB component 
  struct MobComponent
  {
    int mob_id;
  };

  // Component 14: Door component
  struct DoorComponent
  {
    int door_id;
    int dest_scene_id;
    float player_enter_x;
    float player_enter_y;
  };



  // TABLES
  extern phmap::flat_hash_map<unsigned int, ecs::EntityData> entities;
  extern phmap::flat_hash_map<unsigned int, ecs::PositionComponent> positions;
  extern phmap::flat_hash_map<unsigned int, ecs::ModelComponent> models;
  extern phmap::flat_hash_map<unsigned int, ecs::ColorComponent> colors;
  extern phmap::flat_hash_map<unsigned int, ecs::RenderdataComponent> renderdatas;
  extern phmap::flat_hash_map<unsigned int, ecs::ButtonComponent> buttons;
  extern phmap::flat_hash_map<unsigned int, ecs::LabelComponent> labels;
  //extern phmap::flat_hash_map<unsigned int, ecs::MoveComponent> moves;
  extern phmap::flat_hash_map<unsigned int, ecs::StatsComponent> stats;
  extern phmap::flat_hash_map<unsigned int, ecs::CollisionsComponent> collisions;
  extern phmap::flat_hash_map<unsigned int, ecs::SensorsComponent> sensors;
  extern phmap::flat_hash_map<unsigned int, ecs::ItemComponent> items;
  extern phmap::flat_hash_map<unsigned int, ecs::EquipmentComponent> equipments;
  extern phmap::flat_hash_map<unsigned int, ecs::NPCComponent> npcs;
  extern phmap::flat_hash_map<unsigned int, ecs::MobComponent> mobs;
  extern phmap::flat_hash_map<unsigned int, ecs::DoorComponent> doors;


  // METHODS

  // Initializes component system
  void init();

  // Read entity data and return pointer to the struct with data
  ecs::TempEntityData read_data(std::string& file_name);

  // Creates entity and propagates components based on the TempEntityData struct
  // Usage: ecs::TempEntityData entity; ecs::create_entity(&entity);
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
  void _add_model(int entity_id, ecs::ModelComponent model);
  void _add_color(int entity_id, ecs::ColorComponent color);
  void _add_renderdata(int entity_id, ecs::RenderdataComponent renderdata);
  void _add_button(int entity_id, ecs::ButtonComponent button);
  void _add_label(int entity_id, ecs::LabelComponent label);
  //void _add_move(int entity_id, ecs::MoveComponent move);
  void _add_stat(int entity_id, ecs::StatsComponent stat);
  void _add_collision(int entity_id, ecs::CollisionsComponent collision);
  void _add_sensor(int entity_id, ecs::SensorsComponent sensor);
  void _add_item(int entity_id, ecs::ItemComponent item);
  void _add_equipment(int entity_id, ecs::EquipmentComponent equipment);
  void _add_npc(int entity_id, ecs::NPCComponent npc);
  void _add_mob(int entity_id, ecs::MobComponent mob);
  void _add_door(int entity_id, ecs::DoorComponent door);

  // Component dropping methods
  void _drop_position(int entity_id);
  void _drop_model(int entity_id);
  void _drop_color(int entity_id);
  void _drop_renderdata(int entity_id);
  void _drop_button(int entity_id);
  void _drop_label(int entity_id);
  //void _drop_move(int entity_id);
  void _drop_stat(int entity_id);
  void _drop_collision(int entity_id);
  void _drop_sensor(int entity_id);
  void _drop_item(int entity_id);
  void _drop_equipment(int entity_id);
  void _drop_npc(int entity_id);
  void _drop_mob(int entity_id);
  void _drop_door(int entity_id);

  // Where to put it

  // Update entity position and move components
  void update_position(int entity_id, float x, float y);

  // Update entity position and move components based on difference from current position
  void update_position_diff(int entity_id, float x, float y);

  // Just sets position of non moving entity, no fluff
  void set_position(int entity_id, float x, float y);

  // Reverts X position for entity to previous value
  void revert_position_x(int entity_id);

  // Reverts Y position for entity to previous value
  void revert_position_y(int entity_id);

  // Method hiding visible entity
  void hide(int entity_id);

  // Method showing hidden entity
  void show(int entity_id);

  // Saving entity to TempEntityData
  void save_temp(int entity_id);
}


#endif
