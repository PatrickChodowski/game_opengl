
#include <string>

#include "ecs.h"
#include "models.h"
#include "quads.h"

#include "../dictionary.h"
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/parallel_hashmap/btree.h"

#ifndef MODULES_FONTS_H
#define MODULES_FONTS_H

namespace fonts
{  
  extern unsigned int FONT_MODEL_ID;
  extern int ATLAS_CHARACTER_HEIGHT;
  extern float CH_OFFSET;

  // ID of the label for new game name input
  extern int NEW_GAME_LABEL_ID;

  // Addtional mapping from character to frame_id for Font model
  extern phmap::flat_hash_map<char, int>  CharacterAtlas;

  // Read font model
  void init();

  // Render charcters from the Label component
  void render();

  // Render characters of selected label data
  void _render_chars(int entity_id, ecs::LabelComponent label);
}

#endif

