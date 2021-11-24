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
    int gui_popup_id;

    // original space
    float x, y;
    float h, w;
    float r, g, b, a;
    float speed;

    float mid_x, mid_y;
    float diag;
    float camera_type;

    float norm_x_start, norm_x_end;
    
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

  // Entities data -> entity_id, EntityData
  extern std::map<int, EntityData> entities;
  extern std::vector<int> UsedEntityIds;

  // Creates entity and adds it to the table. Uses the data structure different based on entity type
  template <typename T>
  int create(T data, int entity_type_id, float camera_type);

  // Creates quads for the entity table
  void render();

  // Clears entity table
  void clear();

  // Dislays info in Debug Gui popup 
  int display(int entity_id, int gui_slot_id);

}

#endif