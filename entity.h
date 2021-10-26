#include <map>

#ifndef ENTITY_NEW_H
#define ENTITY_NEW_H

namespace entity
{
  struct EntityData
  {
    int entity_id;
    int x;
    int y;
    int texture_id;
    int frame_id;
    int event_id;
    int status;
  };

  std::map<int, EntityData> entities;

  // Spawns entity and inserts it to Entity table
  int spawn();

  // Updates entity in the table
  int update();

  // Deletes entity from the table
  int kill();

  // Clears table
  void clear();

}

#endif