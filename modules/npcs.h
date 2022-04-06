
#include <string>
#include <vector>

#include "entity.h"
#include "quads.h"
#include "utils.h"

#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#ifndef MODULES_NPCS_H
#define MODULES_NPCS_H

// NPC is another example of entity
// Wont be hostile to the player (well at least in the beginning)
// npcs will be kings, earls, jarls etc.
// also traders and other guys
// Dealing with npcs should impact the game world -> they will have memory of interactions
// Major characters will have randomized personality traits

// confident
// questioning-themselves
// hot tempered
// timid
// tactician/strategist
// planner etc
// Cynic 
// Trusting

// Richard Garriott -> ultima online Explore/Create: My Life in Pursuit of New Frontiers, Hidden Worlds, and the Creative Spark
// Play -> Shroud of the avatar
// Ultima Online -> one great simulation of everything
// They will also have relationships with other major characters

// for now lets start norway sweden

// - Get them doing their own things
// - Give them their own agendas
// - Make them react in ways a human would
// - Give them a working memory of things and people around them
// - Give them a history
// - Give them decent dialogue *Make their deaths permanent
// https://www.reddit.com/r/gamedev/comments/2vh5ho/the_trouble_with_npc_interactions_in_rpgs_lessons/


// you can choose to try to assisinate the king -> based on the popularity of the king, different things can happen
// usually guards will stop you and you are in trouble
// sometimes not, maybe you can run away or not
// ypu cam kill blacksmith, the new one will show up in town, but the town will hate you (everybody needs blacksmith)

// death -> you go to valhalla and then odin says thats its not your time yet (?)

// first motive -> your father goes to raid, you are not ready yet, he doesnt come back, you have a dream he tells you he couldnt reach valhalla -> something happened 

namespace npcs
{

// read_data
// init
// table of events -> event_id, +/-, value, alliance_id
// recalculate sentiments on each event
// hashmap of checks of what was done for who
// different valus based on the sentiments of two kingdoms

  struct NPCData
  {
    int id;
    int entity_id;
    int model_id;
    int current_frame;
    int personality_id;
    int scene_id;

    float x;
    float y;
    float w;
    float h;
    float speed;
    float sentiment = 0.5;


    std::string name;

    JS_OBJ(id, model_id, current_frame, w, h, speed, name);
  };


  struct InteractionData
  {
    int event_type_id;
    int npc_id;
    float type; // positive +1, negative -1;
    float value;

    JS_OBJ(event_type_id, npc_id, type, value);
  };

  // Catalog of all npcs
  extern phmap::flat_hash_map<int, NPCData> npcs_data;

  // Current npcs
  extern phmap::flat_hash_map<int, NPCData> npcs;

  // Table of history of interactions
  extern std::vector<InteractionData> interactions;

  // Read npc data from the file
  void read_data(std::string& name);

  // Read all npc data files and fill npcs_data
  void init();

  // Spawn npc, adds instance to npcs map. Returns entity id
  int spawn(int npc_id, float x, float y); 

  // Spawn group of npcs assigned to the map
  void spawn_from_map(int map_id);

  // Clear temporary data (npcs)
  void clear();

  // Clear catalog
  void refresh();
  
  // Create interaction with the entity. impact interactions table and NPC's sentiment
  void interact(int entity_id, float value);

  // Returns vector of strings with npc information
  std::vector<std::string> info(int entity_id);
  
}

#endif
