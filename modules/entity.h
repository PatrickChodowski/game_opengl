#include "quads.h"

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

    // original space
    float x;
    float y;
    float h;
    float w;
    float mid_x;
    float mid_y;
    float diag;

    // scaled space
    float s_x;
    float s_y;
    float s_h;
    float s_w;
    float s_mid_x;
    float s_mid_y;
    float s_diag;

    float camera_type;

    bool is_solid;
    bool is_clicked;

    // used if given entity has collision sensors
    std::map<int, collisions::Sensor> sensors;
    // its not abs, its AABBs, but this is what I do to entertain myself
    std::map<int, collisions::AABB> abs; 

    // label collection
    // std::vector<QuadLabel> labels;
  };

  extern std::vector<quads2::QuadData> EntityQuads;
  extern std::map<int, EntityData> entities;
  extern std::vector<int> UsedEntityIds;

  // Creates entity and adds it to the table. Uses the data structure different based on entity type
  template <typename T>
  int create(T data, int entity_type_id, float camera_type);

  // Updates entity in the table
  int update(int entity_id);

  // Deletes entity from the table
  int drop(int entity_id);

  // Creates quads for the entity table
  void render();

  // Clears entity table
  void clear();

  // Find next available entity id
  int _find_next();

  // Generate next entity id (calls _find_next and inserts it to UsedEntityIds)
  int _gen_entity_id();

}

#endif