#include "quad.h"

#ifndef MODULES_ENTITY_H
#define MODULES_ENTITY_H

// Persistent data. Table of entities and all operations on this table
namespace entity
{

  // Contains all necessary entity information 
  struct EntityData
  {
    int entity_id;
    int texture_id;
    int frame_id;
    int event_id;
    int status;

    float x;
    float y;
    float h;
    float w;
    float mid_x;
    float mid_y;

    float diag;
  };

  extern std::vector<quad::QuadData> EntityQuads;
  extern std::map<int, EntityData> entities;
  extern std::vector<int> UsedEntityIds;

  // Creates entity and adds it to the table
  int create();

  // Updates entity in the table
  int update(int entity_id);

  // Deletes entity from the table
  int drop(int entity_id);

  // Renders entity (creates quad for entity)
  int render(int entity_id);

  // Clears entity table
  void clear();

  // Find next available entity id
  int _find_next();

  // Generate next entity id (calls _find_next and inserts it to UsedEntityIds)
  int _gen_entity_id();

}

#endif