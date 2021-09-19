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

    // assign map quads
    if(maps::MapQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), maps::MapQuads.begin(), maps::MapQuads.end());
    }

    // assign text quads
    // if(fonts::TextQuads.size() > 0){
    //   quads::AllQuads.insert(quads::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    // }

    // assign entity quads
    if(ent::EntityQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), ent::EntityQuads.begin(), ent::EntityQuads.end());
    }

    // assign gui quads
    if(gui::GuiQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), gui::GuiQuads.begin(), gui::GuiQuads.end());
    }

        if(fonts::TextQuads.size() > 0){
      quads::AllQuads.insert(quads::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    }


    // quads::print_out_quads(quads::AllQuads);
    quads::QuadsSummary["map"] = maps::MapQuads.size();
    quads::QuadsSummary["menu"] = menu::MenuQuads.size();
    quads::QuadsSummary["text"] = fonts::TextQuads.size();
    quads::QuadsSummary["entity"] = ent::EntityQuads.size();
    quads::QuadsSummary["gui"] = gui::GuiQuads.size();

    quads::COUNT_QUADS = quads::AllQuads.size();
    quads::REQ_SIZE_BUFFER = COUNT_QUADS*6*sizeof(float);
  }


  void scale_move_quads(int camera_x=0, int camera_y=0, float camera_zoom=1.0)
  // works on quads::AllQuads data, provides s_x, s_y, s_w, s_h, radius based on the camera move and tile type
  // scaled information is used for collisions, nav mesh, mouse events
  // scaling only quads from map and entity (probably will need to scale some texts later) but! later! not now
  {
    float scale_factor = (1.0f/float(camera_zoom));
    camera::tile_dim = (float)TILE_DIM*scale_factor;
    camera_x = (-1)*camera_x;

    for(int q=0; q<quads::AllQuads.size(); q++)
    {
      if (quads::AllQuads[q].type_id == QUAD_TYPE_MAP || quads::AllQuads[q].type_id == QUAD_TYPE_ENTITY){
        if(quads::AllQuads[q].entity_type_id != ENTITY_TYPE_ID_HERO)
        {
          quads::AllQuads[q].s_x  = ((float)quads::AllQuads[q].x + (float)camera_x)*scale_factor;
          quads::AllQuads[q].s_y  = ((float)quads::AllQuads[q].y + (float)camera_y)*scale_factor;
          quads::AllQuads[q].s_h  =  (float)quads::AllQuads[q].h*scale_factor;
          quads::AllQuads[q].s_w  =  (float)quads::AllQuads[q].w*scale_factor;
          quads::AllQuads[q].s_diag = std::sqrt(std::pow((quads::AllQuads[q].s_w/2),2) + std::pow((quads::AllQuads[q].s_h/2),2));
        } else if (quads::AllQuads[q].entity_type_id == ENTITY_TYPE_ID_HERO) {
          quads::AllQuads[q].s_x =  ((float)quads::AllQuads[q].x)*scale_factor;
          quads::AllQuads[q].s_y  = ((float)quads::AllQuads[q].y)*scale_factor;
          quads::AllQuads[q].s_h  =  (float)quads::AllQuads[q].h*scale_factor;
          quads::AllQuads[q].s_w  =  (float)quads::AllQuads[q].w*scale_factor;
          quads::AllQuads[q].s_diag = std::sqrt(std::pow((quads::AllQuads[q].s_w/2),2) + std::pow((quads::AllQuads[q].s_h/2),2));
        }
      }
    }

    for(int m=0; m<mobs::AliveMobs.size(); m++)
    {
      mobs::AliveMobs[m].s_x = ((float)mobs::AliveMobs[m].x + (float)camera_x)*scale_factor;
      mobs::AliveMobs[m].s_y = ((float)mobs::AliveMobs[m].y + (float)camera_y)*scale_factor;
    }
  };

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

  void set_abs(std::vector<colls::DistanceBetweenPoints> near_distances)
  {   
    // for each entity in near distance set the aabb
    for(int i = 0; i < near_distances.size(); i++)
    {
      int qid = quads::find_quad_id(near_distances[i].b_quad_id, quads::AllQuads);
      quads::AllQuads[qid].abs.clear();
      // for now just producing one box to not overcomplicate
      for(int a = 0; a < colls::ABS_COUNT; a++)
      {
        colls::AABB aabb;
        aabb.min_x = (quads::AllQuads[qid].s_x + colls::SENSOR_OFFSET);
        aabb.min_y = (quads::AllQuads[qid].s_y + colls::SENSOR_OFFSET);
        aabb.max_y = (quads::AllQuads[qid].s_y + quads::AllQuads[qid].s_h - colls::SENSOR_OFFSET);
        aabb.max_x = (quads::AllQuads[qid].s_x + quads::AllQuads[qid].s_w - colls::SENSOR_OFFSET);
        aabb.id = AABB_FULL;
        quads::AllQuads[qid].abs.insert(std::pair<int, colls::AABB>(AABB_FULL, aabb));
      }
    }
  };

  // Each quad can have set of labels to be moved with the quad.. right?
  void set_labels()
  {
    // for(int q = 0; q < quads::AllQuads.size(); q++)
    // {
      // if(quads::AllQuads[q].entity_type_id == ENTITY_TYPE_ID_MOB)
      // {
      //   QuadLabel ql;
      //   ql.text = std::to_string(quads::AllQuads[q].id) + "_(" + std::to_string(quads::AllQuads[q].x) + "," + std::to_string(quads::AllQuads[q].y) + ")";
      //   ql.x = quads::AllQuads[q].x + 20;
      //   ql.y = quads::AllQuads[q].y - 10;
      //   ql.is_static = 0.0f;
      //   ql.scale = 0.5;
      //   quads::AllQuads[q].labels.push_back(ql);


      //   QuadLabel ql0;
      //   ql0.text = "scaled:_(" + std::to_string((int)quads::AllQuads[q].s_x) + "," + std::to_string((int)quads::AllQuads[q].s_y) + ")";
      //   ql0.x = quads::AllQuads[q].x + 20;
      //   ql0.y = quads::AllQuads[q].y + quads::AllQuads[q].h + 20;
      //   ql0.is_static = 0.0f;
      //   ql0.scale = 0.5;
      //   quads::AllQuads[q].labels.push_back(ql0);

      // }
   // }
  }

  // Assign label to quad by ID, x, y, text, scale, is_static
  void add_label(int quad_id, float x, float y, std::string text, float scale, bool is_static)
  {
    int q = quads::find_quad_id(quad_id, quads::AllQuads);
    QuadLabel ql;
    ql.text = text;
    ql.x = x;
    ql.y = y;
    ql.is_static = is_static;
    ql.scale = scale;
    quads::AllQuads[q].labels.push_back(ql);
  }

  
}


#endif
