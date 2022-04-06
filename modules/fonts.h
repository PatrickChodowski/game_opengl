
#include <string>

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
  extern std::string FONT_NAME;

  // Persistent across frames for both replaceable and nonreplaceble information.
  // Stores whole text, not single character
  struct LabelData 
  {
    int id;
    int size;

    float r, g, b;
    float x_start;
    float y_start;
    float camera_type;
    std::string text;
  };


  // Stores in-game character data -> text, id, position, color. Used to make quads
  struct TextCharacterData
  {
    int id;
    int model_id = FONT_MODEL_ID;
    int frame_id;

    quads::Color color;
    quads::Position pos;
    quads::Dims dims;

    float camera_type;
    float char_width;
    float char_height;
    float atlas_width;
    float atlas_height;
    float offset;

    bool is_clicked;
    bool is_reversed = false;
  };

  // Font frame information
  struct FontsFrameData
  {
    char* frame_id;
    char  label;

    int x;
    int y;
    int w;
    int h;

    float norm_x_start;
    float norm_x_end;
    float norm_y_start;
    float norm_y_end;

    JS_OBJ(label, x, y, w, h, norm_x_start, norm_x_end, norm_y_start, norm_y_end);
  };


  // Special version of model struct, adjusted for Font texture
  struct FontsModelData
  {
    int id = FONT_MODEL_ID;
    int w;
    int h;
    unsigned int opengl_texture_id;
    std::vector<fonts::FontsFrameData> frames_list;

    std::string name;
    phmap::btree_map<int, fonts::FontsFrameData> frames;
    JS_OBJ(id, w, h, name, frames_list);
  };

  // Vector of used Text Ids
  extern std::vector<int> TextIndex;

  // Vector of used Label Ids
  extern std::vector<int> LabelIndex;

  // Map of single character id, TextData (single characters stored)
  extern phmap::flat_hash_map<int, fonts::TextCharacterData> texts;

  // Map of Level's labels stored across frames
  extern phmap::flat_hash_map<int, fonts::LabelData> labels;

  // Font's texture data 
  extern fonts::FontsModelData FontTDD;

  // ID of the label for new game name input
  extern int NEW_GAME_LABEL_ID;




  // Read font model
  void init();

  // Reads in font model data
  void read_data(std::string& file_name);

  // Loads texture to opengl
  unsigned int _load_font_texture_to_opengl(int w, int h, int n_channels);

  // Add new label to labels. Returns label_id
  int add(std::string& text, float x_start, float y_start, float camera_type, int size, float r, float g, float b);

  // Render characters of selected label data
  void render_chars(fonts::LabelData ldd);

  // Render charcters from the texts map
  void render();

  // Clear all texts related data
  void clear();

  // Deletes label by label id
  void drop(int label_id);


}

#endif

