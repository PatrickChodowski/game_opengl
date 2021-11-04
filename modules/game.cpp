#include "game.h"

#include "buffer.h"
#include "quads.h"
#include "shaders.h"
#include "textures.h"

namespace game2
{
  bool RUNNING = true;
  int CURRENT_SHADER_ID = 0;

  void init()
  {
    buffer2::init();
    shaders2::init();
    textures2::init();
    
  };

  void update()
  {

    quads2::accumulate();
    textures2::bind();
    buffer2::update(quads2::AllQuads);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures2::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures2::BoundTextures.size(); i++)
    {
      sampler[(i+1)] = textures2::BoundTextures[i];
    }
    // react to camera changes
    // DYNAMIC_MVP = camera::generate_dynamic_mvp(camera::zoom, -camera::x, camera::y);

    // // independent of camera moving
    // STATIC_MVP = camera::generate_static_mvp();

    // // zoom only for hero?
    // ZOOM_MVP = camera::generate_zoom_only_mvp(camera::zoom);
  
    // // set light source coordinates
    // glUniform3fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "light_coords"), 1, light_coords);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    // glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "static_mvp"), 1, GL_FALSE, glm::value_ptr(STATIC_MVP));
    // glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "dynamic_mvp"), 1, GL_FALSE, glm::value_ptr(DYNAMIC_MVP));
    // glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "zoom_mvp"), 1, GL_FALSE, glm::value_ptr(ZOOM_MVP));
    
    // set shader
    glUseProgram(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id);
    // draw 
    glDrawElements(GL_TRIANGLES, quads2::AllQuads.size()*6, GL_UNSIGNED_INT, nullptr);

  }


}
