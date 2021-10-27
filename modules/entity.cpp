#include <ostream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include "entity.h"

namespace entity
{

  std::map<int, EntityData> entities;
  std::vector<int> UsedEntityIds = {};
  std::vector<quad::QuadData> EntityQuads;

  int create()
  { 

    std::cout << "spawn" << std::endl;

    return 0;
  }

  int update(int entity_id)
  { 

    std::cout << "update" << std::endl;

    return entity_id;
  }

  int drop(int entity_id)
  { 
    entity::UsedEntityIds.erase(std::remove(entity::UsedEntityIds.begin(), 
                                            entity::UsedEntityIds.end(), entity_id), 
                                            entity::UsedEntityIds.end());
    entity::entities.erase(entity_id);
    return entity_id;
  }

  int render(int entity_id)
  {
    std::cout << "render" << std::endl;

    return 0;
  }

  void clear()
  {
    entity::UsedEntityIds.clear();
    entity::entities.clear();
  }

  int _find_next()
  {
    int n = entity::UsedEntityIds.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (entity::UsedEntityIds[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int _gen_entity_id()
  {
    int next_entity_id = entity::_find_next();
    entity::UsedEntityIds.push_back(next_entity_id);
    return next_entity_id;
  }

}
