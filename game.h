#ifndef GAME_H
#define GAME_H


// all the loop/scene/game view related stuff I hope
namespace game
{

  void update(std::vector<quads::Quad> quads)
  {
    textures::bind_all();
    buffer::update(quads);

    // glClear(GL_COLOR_BUFFER_BIT); // black by default
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // sampler array creation
    int sampler_size = (textures::BoundTextures.size() + 1);
    int sampler[sampler_size]; 
    sampler[0] = 0;
    for (int i = 0; i < textures::BoundTextures.size(); i++) {
      sampler[(i+1)] = textures::BoundTextures[i];
    }

    // react to camera changes
    glm::mat4 MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);

    // this should react to map quads only ?
    quads::ScaledAllQuads = camera::scale_move_quads(quads, -camera::x, camera::y);

    // set uniforms
    glUniform1iv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
    glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    
    // set shader
    glUseProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
    // draw 
    glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);
  }

}




#endif