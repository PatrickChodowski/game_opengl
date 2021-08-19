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

    // assign entity quads
    if(ent::EntityQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), ent::EntityQuads.begin(), ent::EntityQuads.end());
    }

    // quads::print_out_quads(quads::AllQuads);
    quads::QuadsSummary["map"] = maps::MapQuads.size();
    quads::QuadsSummary["menu"] = menu::MenuQuads.size();
    quads::QuadsSummary["text"] = fonts::TextQuads.size();
    quads::QuadsSummary["entity"] = fonts::TextQuads.size();
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

    if(quad_type_id == QUAD_TYPE_MAP){
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
    } else if (quad_type_id == QUAD_TYPE_MENU){
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

  void all_quads_to_tsv_file()
  {
    std::string file_path = "./logs/all_quads.txt";
    std::ofstream quads_file (file_path.c_str());

    if (quads_file.is_open())
    {
      for(int i = 0; i < quads::AllQuads.size(); i++)
      {
        quads_file << quads::AllQuads[i].id << " " << 
                      quads::AllQuads[i].x << " " << 
                      quads::AllQuads[i].y << " " << 
                      quads::AllQuads[i].w << " " << 
                      quads::AllQuads[i].h << " " << 
                      quads::AllQuads[i].solid << " " << 
                      quads::AllQuads[i].coll << " " << 
                      quads::AllQuads[i].is_clicked << " " << 
                      quads::AllQuads[i].a << " " << 
                      quads::AllQuads[i].b << " " << 
                      quads::AllQuads[i].c << " " << 
                      quads::AllQuads[i].d << " " << 
                      quads::AllQuads[i].texture_id << " " << 
                      quads::AllQuads[i].frame_id << " " << 
                      quads::AllQuads[i].r_col << " " << 
                      quads::AllQuads[i].g_col << " " << 
                      quads::AllQuads[i].b_col << " " << 
                      quads::AllQuads[i].a_col << " " << 
                      quads::AllQuads[i].type_id << " " << 
                      quads::AllQuads[i].is_static << " " << 
                      quads::AllQuads[i].entity_type_id << " " << 
                      quads::AllQuads[i].alive << " " << 
                      quads::AllQuads[i].s_x << " " << 
                      quads::AllQuads[i].s_y << " " << 
                      quads::AllQuads[i].s_w << " " << 
                      quads::AllQuads[i].s_h << " " << 
                      quads::AllQuads[i].s_diag << " " << "\n";
      }
      quads_file.close();
    }
  }



  void all_quads_to_json()
  {
    std::string file_path = "./logs/all_quads.json";
    std::ofstream quads_file (file_path.c_str());
    std::string end_str = " }, \n";
    if (quads_file.is_open())
    {
      quads_file << "[ \n";
      for(int i = 0; i < quads::AllQuads.size(); i++)
      {
        if(i == (quads::AllQuads.size() - 1)){
          // last quad
          end_str = " } \n";
        }
        quads_file << " { \n" <<
                      "    \"id\": " << quads::AllQuads[i].id                         << ",\n"
                      "    \"x\": " << quads::AllQuads[i].x                           << ",\n"
                      "    \"y\": " << quads::AllQuads[i].y                           << ",\n"
                      "    \"w\": " << quads::AllQuads[i].w                           << ",\n"
                      "    \"h\": " << quads::AllQuads[i].h                           << ",\n"
                      "    \"solid\": " << quads::AllQuads[i].solid                   << ",\n"
                      "    \"coll\": " << quads::AllQuads[i].coll                     << ",\n"
                      "    \"is_clicked\": " << quads::AllQuads[i].is_clicked         << ",\n"
                      "    \"a\": " << quads::AllQuads[i].a                           << ",\n"
                      "    \"b\": " << quads::AllQuads[i].b                           << ",\n"
                      "    \"c\": " << quads::AllQuads[i].c                           << ",\n"
                      "    \"d\": " << quads::AllQuads[i].d                           << ",\n"
                      "    \"texture_id\": " << quads::AllQuads[i].texture_id         << ",\n"
                      "    \"frame_id\": " << quads::AllQuads[i].frame_id             << ",\n"
                      "    \"r_col\": " << quads::AllQuads[i].r_col                   << ",\n"
                      "    \"g_col\": " << quads::AllQuads[i].g_col                   << ",\n"
                      "    \"b_col\": " << quads::AllQuads[i].b_col                   << ",\n"
                      "    \"a_col\": " << quads::AllQuads[i].a_col                   << ",\n"
                      "    \"type_id\": " << quads::AllQuads[i].type_id               << ",\n"
                      "    \"is_static\": " << quads::AllQuads[i].is_static           << ",\n"
                      "    \"entity_type_id\": " << quads::AllQuads[i].entity_type_id << ",\n"
                      "    \"alive\": " << quads::AllQuads[i].alive                   << ",\n"
                      "    \"s_x\": " << quads::AllQuads[i].s_x                       << ",\n"
                      "    \"s_y\": " << quads::AllQuads[i].s_y                       << ",\n"
                      "    \"s_w\": " << quads::AllQuads[i].s_w                       << ",\n"
                      "    \"s_h\": " << quads::AllQuads[i].s_h                       << ",\n"
                      "    \"s_diag\": " << quads::AllQuads[i].s_diag                 << "\n" 
                      << end_str;
      }
      quads_file << "] \n";
      quads_file.close();
    }
  }
}


#endif
