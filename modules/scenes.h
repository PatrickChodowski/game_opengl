

#include <map>
#include <string>
#include <vector>

#include "../dictionary.h"
#include "../dependencies/json_struct.h"

#ifndef MODULES_SCENES_H
#define MODULES_SCENES_H

// Handles spawning of initial menus, entities and picking handlers 
// All dependent on the scene_id. Defined in the data files 
// makes it easy to load/unload levels and spawn new levels.
namespace scenes
{
  struct SceneData 
  {
    int id;
    int events_handler_id;
    int map_id;
    std::vector<int> menu_slots;
    std::vector<int> menu_types;
    float hero_start_x, hero_start_y;
    std::string label;

    JS_OBJ(id, events_handler_id, map_id, 
    hero_start_x, hero_start_y, menu_slots, menu_types, label);
  };

  extern int EVENT_HANDLER_ID;
  extern int MAP_ID;
  extern float HERO_START_X;
  extern float HERO_START_y;
  extern std::map<int, scenes::SceneData> scenes;

  // Reads scene data to struct
  void read_data(std::string &name);

  // Reads all scenes data to scenes catalog
  void init();

  // Loads new scene to memory
  void load(int scene_id);

  // Clears the scene completely
  void clear();

  // Change scene to new one
  void change(int scene_id);

  // Clear the catalogs
  void data_clear();

  // Clear and re-init the catalogs
  void data_refresh();

}


#endif
