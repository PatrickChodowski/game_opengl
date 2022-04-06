#include "fonts.h"
#include "models.h"
#include "utils.h"

#include <iostream>

#include <string>
#include <vector>

// Opengl packages
#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"


namespace fonts
{
  unsigned int FONT_MODEL_ID = 1;
  std::vector<int> TextCharacterIndex;
  std::vector<int> LabelIndex;
  phmap::flat_hash_map<int, fonts::TextCharacterData> text_characters;
  phmap::flat_hash_map<int, fonts::LabelData> labels;
  phmap::flat_hash_map<char, int>  CharacterAtlas;
  int ATLAS_CHARACTER_HEIGHT;
  int NEW_GAME_LABEL_ID;
  float CH_OFFSET = 0;


  void init()
  {
    for(int f=0; f < models::models.at(FONT_MODEL_ID).frames_list.size(); f++)
    {
      char text_chr = models::models.at(FONT_MODEL_ID).frames_list[f].label[0]; // IT CAN ALWAYS BE ONLY ONE CHARACTER
      int frame_id = models::models.at(FONT_MODEL_ID).frames_list[f].frame_id;
      fonts::CharacterAtlas.insert(std::pair<char, int>{text_chr, frame_id});
    }
    fonts::ATLAS_CHARACTER_HEIGHT = models::models.at(FONT_MODEL_ID).frames[0].h;
    models::load(FONT_MODEL_ID);
    std::cout << "Fonts Initialized" << std::endl;
  }

  int add(std::string& text, float x_start, float y_start, float camera_type, float size, float r, float g, float b)
  {
    fonts::LabelData ldd;
    ldd.id = utils::generate_id(fonts::LabelIndex);
    ldd.text = text;
    ldd.x_start = x_start;
    ldd.y_start = y_start;
    ldd.camera_type = camera_type;
    ldd.r = r;
    ldd.g = g;
    ldd.b = b;
    ldd.size = size;
    fonts::labels[ldd.id] = ldd;
    return ldd.id;
  };

  void render_chars(fonts::LabelData ldd)
  {
    float x = ldd.x_start;
    float y = ldd.y_start;
    float scale = ldd.size/fonts::ATLAS_CHARACTER_HEIGHT;
    models::ModelData FMD = models::models.at(FONT_MODEL_ID);
    for(std::string::size_type i = 0; i < ldd.text.size(); i++) 
    { 
      char p = ldd.text[i];
      fonts::TextCharacterData tdd;
      tdd.id = utils::generate_id(fonts::TextCharacterIndex);
      tdd.model_id = fonts::FONT_MODEL_ID;
      tdd.frame_id = 0;
      tdd.frame_id = fonts::CharacterAtlas[p];
      tdd.pos.x = x;
      tdd.pos.y = y;
      tdd.pos.z = 0.95f;
      tdd.dims.w = FMD.frames.at(tdd.frame_id).w * scale;
      tdd.dims.h = FMD.frames.at(tdd.frame_id).h * scale;
      tdd.color.r = ldd.r;
      tdd.color.g = ldd.g;
      tdd.color.b = ldd.b;
      tdd.color.a = 1.0;
      tdd.camera_type = ldd.camera_type;
      tdd.is_clicked = false;

      x += tdd.dims.w + (fonts::CH_OFFSET*scale);
      fonts::text_characters[tdd.id] = tdd;
    }
  };

  void render()
  {
    fonts::TextCharacterIndex.clear();
    fonts::text_characters.clear();

    for (auto const& [k, v] : fonts::labels)
    { 
      fonts::render_chars(v);
    } 

    //std::cout << "before quads::add_quads" << std::endl;
    quads::add_quads(fonts::text_characters, OBJECT_TYPE_TEXT);
  }

  void clear()
  {
    fonts::text_characters.clear();
    fonts::labels.clear();
    fonts::TextCharacterIndex.clear();
    fonts::LabelIndex.clear();
    fonts::NEW_GAME_LABEL_ID = -1;
  };


  void drop(int label_id)
  {
    fonts::labels.erase(label_id);
    utils::drop_id(fonts::LabelIndex, label_id);
  };





}