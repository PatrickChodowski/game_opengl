#ifndef QUADS_MANAGER_H
#define QUADS_MANAGER_H

// To have all quads managed in one space
// Menu Quads
// Level Quads
// Entity quads 
// etc.


namespace qm
{

  void accumulate(std::vector<quads::Quad> menu_quads,
                  std::vector<quads::Quad> level_quads,
                  std::vector<quads::Quad> text_quads)
  {
    AllQuads.clear(); // resetting map

    // assign menu quads
    if(menu_quads.size() > 0){

      for(int m=0; m < menu_quads.size(); m++)
      {
        AllQuads[menu_quads[m].id] = menu_quads[m];
      }
    }

    // assign level quads
    if(level_quads.size() > 0){
      for(int l=0; l < level_quads.size(); l++)
      {
        AllQuads[level_quads[l].id] = level_quads[l];
      }
    }

    // assign text quads
    if(text_quads.size() > 0){
      for(int l=0; l < text_quads.size(); l++)
      {
        AllQuads[text_quads[l].id] = text_quads[l];
      }
    }

    //quads::print_out_quad_map(all_quads);
    quads_sizes["menu"] = menu_quads.size();
    quads_sizes["level"] = level_quads.size();
    quads_sizes["text"] = text_quads.size();

  }


  int gen_quad_id()
  {
    // is supposed to return new quad id based on qm.next_quad_id
    // yes, kinda updates namespace's global variable
    qm::next_quad_id += 1;
    logger::print("Next Quad ID: "+std::to_string(qm::next_quad_id));
    return qm::next_quad_id;
  }



  // void update(std::map<int, quads::Quad> all_quads)
  // {
  //   textures::bind_all();
  //   // just test::
  //   //std::vector<quads::Quad> quads = quads::assign_vertices(quads_vector);
  //   std::vector<quads::Quad> quads = quads_vector;
  //   buffer::update(quads_vector);
  //   // test finish


  //   glClear(GL_COLOR_BUFFER_BIT);

  //   // sampler array creation
  //   int sampler_size = (textures::BoundTextures.size() + 1); //3
  //   int sampler[sampler_size]; 
  //   sampler[0] = 0;
  //   for (int i = 0; i < textures::BoundTextures.size(); i++) {
  //     sampler[(i+1)] = textures::BoundTextures[i];
  //   }

  //   // react to camera changes
  //   glm::mat4 MVP = camera::generate_mvp(camera::zoom, -camera::x, camera::y);
  //   ScaledLevelQuads = camera::scale_move_quads(quads_vector, -camera::x, camera::y);

  //   // set uniforms
  //   glUniform1iv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "textures"), sampler_size, sampler);
  //   glUniformMatrix4fv(glGetUniformLocation(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id, "mvp"), 1, GL_FALSE, glm::value_ptr(MVP));
    
  //   // set shader
  //   glUseProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
  //   // bind buffer
  //   glBindVertexArray(buffer::VAO); 
  //   // draw 
  //   glDrawElements(GL_TRIANGLES, quads.size()*6, GL_UNSIGNED_INT, nullptr);






  // }













}



  #endif