#include "ecs.h"
#include "fonts.h"
#include "models.h"
#include "quads.h"
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

  void _render_chars(int entity_id, ecs::LabelComponent label)
  {
    float x = label.text_x;
    float y = label.text_y;
    float scale = label.text_size/fonts::ATLAS_CHARACTER_HEIGHT;
    models::ModelData FMD = models::models.at(FONT_MODEL_ID);

    for(std::string::size_type i = 0; i < label.label.size(); i++) 
    { 
      char p = label.label[i];
      quads::QuadData quad;
      quad.entity_id = entity_id;
      quad.model_id = fonts::FONT_MODEL_ID;
      quad.frame_id = fonts::CharacterAtlas[p];

      if(quad.model_id > -1){
        quad.texture_id = models::SceneModels.at(quad.model_id);
        quad.norm_x_start = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_start;
        quad.norm_x_end = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_x_end;
        quad.norm_y_start = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_start;
        quad.norm_y_end = models::models.at(quad.model_id).frames.at(quad.frame_id).norm_y_end;
      }

      quad.entity_type_id = ENTITY_TYPE_TEXT;
      quad.camera_type = ecs::renderdatas.at(entity_id).camera_type;
      quad.r = label.text_r;
      quad.g = label.text_g;
      quad.b = label.text_b;
      quad.a = label.text_a;
      quad.x = label.text_x;
      quad.y = label.text_y;
      quad.z = label.text_z;
      quad.w = FMD.frames.at(quad.frame_id).w * scale;
      quad.h = FMD.frames.at(quad.frame_id).h * scale;

      quad.window_x = quad.x;
      quad.window_y = quad.y;
      quad.window_h = quad.h;
      quad.window_w = quad.h;

      quad.is_clicked = ecs::renderdatas.at(entity_id).is_clicked;
      quad.is_deleted = false;
      quads::AllQuads.push_back(quad);
      x += quad.w + (fonts::CH_OFFSET*scale);
    }
  };

  void render()
  {
    for (auto const& [entity_id, label_data] : ecs::labels)
    { 
      fonts::_render_chars(entity_id, label_data);
    } 
  }

}
