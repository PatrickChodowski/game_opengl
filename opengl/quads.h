#ifndef QUADS_H
#define QUADS_H

// To have all quads managed in one space
// Menu Quads
// Map Quads
// Entity quads 
// etc.


namespace quads
{

  void accumulate()

  {
    quads::AllQuads.clear(); // resetting vector

    // assign menu quads
    if(menu::MenuQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
    }

        // assign level quads
    if(maps::MapQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), maps::MapQuads.begin(), maps::MapQuads.end());
    }

    // assign text quads
    if(fonts::TextQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    }

    // quads::print_out_quads(quads::AllQuads);
    quads::QuadsSummary["map"] = maps::MapQuads.size();
    quads::QuadsSummary["menu"] = menu::MenuQuads.size();
    quads::QuadsSummary["text"] = fonts::TextQuads.size();
  }

  void delete_quad_id(int quad_id)
  {
    //std::cout << "Removing " << quad_id << " from used quads" << std::endl;
    quads::UsedQuadIds.erase(std::remove(quads::UsedQuadIds.begin(), 
                                         quads::UsedQuadIds.end(), quad_id), 
                                         quads::UsedQuadIds.end());

    //std::cout << "Used quads size: " << quads::UsedQuadIds.size() << std::endl;
  }

  void delete_vertex_id(int vertex_id)
  {
    //std::cout << "Removing " << vertex_id << " from used vertexes" << std::endl;
    quads::UsedVertexIds.erase(std::remove(quads::UsedVertexIds.begin(), 
                                           quads::UsedVertexIds.end(), vertex_id), 
                                           quads::UsedVertexIds.end());

    //std::cout << "Used Vertexes size: " << quads::UsedVertexIds.size() << std::endl;
  }

  int find_quad_id(int quad_id, std::vector<quads::Quad> quads)
  {
    int quad_index = -1;
    /*Will return the index of quad id*/
    for(int q = 0; q < quads.size(); q++)
    {
      if(quad_id == quads[q].id)
      {
        quad_index = q;
        break;
      }
    }
    return quad_index;
  }

  void click(int quad_id, int quad_type_id)
  {

    if(quad_type_id == 0){
      // map
      int index = find_quad_id(quad_id, maps::MapQuads);
      if(maps::MapQuads[index].is_clicked == 0.0f)
      {
        maps::MapQuads[index].is_clicked = 1.0f;
        maps::MapQuads[index].v_a.is_clicked = 1.0f;
        maps::MapQuads[index].v_b.is_clicked = 1.0f;
        maps::MapQuads[index].v_c.is_clicked = 1.0f;
        maps::MapQuads[index].v_d.is_clicked = 1.0f;
      } else {
        maps::MapQuads[index].is_clicked = 0.0f;
        maps::MapQuads[index].v_a.is_clicked = 0.0f;
        maps::MapQuads[index].v_b.is_clicked = 0.0f;
        maps::MapQuads[index].v_c.is_clicked = 0.0f;
        maps::MapQuads[index].v_d.is_clicked = 0.0f;
      }
    } else if (quad_type_id == 1){
       // menu
      int index = find_quad_id(quad_id, menu::MenuQuads);
      if(menu::MenuQuads[index].is_clicked == 0.0f)
      {
        menu::MenuQuads[index].is_clicked = 1.0f;
        menu::MenuQuads[index].v_a.is_clicked = 1.0f;
        menu::MenuQuads[index].v_b.is_clicked = 1.0f;
        menu::MenuQuads[index].v_c.is_clicked = 1.0f;
        menu::MenuQuads[index].v_d.is_clicked = 1.0f;
      } else {
        menu::MenuQuads[index].is_clicked = 0.0f;
        menu::MenuQuads[index].v_a.is_clicked = 0.0f;
        menu::MenuQuads[index].v_b.is_clicked = 0.0f;
        menu::MenuQuads[index].v_c.is_clicked = 0.0f;
        menu::MenuQuads[index].v_d.is_clicked = 0.0f;
      }
    }
  }
}


#endif
