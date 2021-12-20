#include "quads.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_ENTITY_H
#define MODULES_ENTITY_H

// Persistent data. Table of entities and all operations on this table
namespace entity
{

  // Contains all necessary entity information 
  struct EntityData
  {
    int id;
    int texture_id;
    int frame_id;
    int event_id;
    int status;
    int entity_type_id;
    int menu_id;

    quads::Position pos;
    quads::Color color;
    quads::Dims dims;
    quads::Norm norm;

    float prev_x, prev_y;
    float speed;
    float mid_x, mid_y;
    float diag;
    float camera_type;
    
    bool is_solid;
    bool is_clicked;

    // used if given entity has collision sensors
    phmap::flat_hash_map<int, collisions::Sensor> sensors;
    // its not abs, its AABBs, but this is what I do to entertain myself
    phmap::flat_hash_map<int, collisions::AABB> abs; 

  };

  // Entities data -> entity_id, EntityData
  extern phmap::flat_hash_map<int, EntityData> entities;
  extern std::vector<int> Index;
  extern phmap::flat_hash_map<int, int> menu_entity_type_map;

  // Creates entity and adds it to the table. Uses the data structure different based on entity type. Returns entity_id
  template <typename T>
  int create(T data, int entity_type_id, float camera_type);

  // Creates quads for the entity table
  void render();

  // Initialize menu_entity_type_map
  void init();
  
  // Clears entity table
  void clear();

  // Update entity's frame
  void update_frame(int entity_id, int frame_id);

  // Update entity's position
  void update_position(int entity_id, float x, float y);

  // Returns vector of strings with entity information
  std::vector<std::string> info(int entity_id);

  // Delete entity
  void drop(int entity_id);

}

#endif