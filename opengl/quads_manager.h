#ifndef QUADS_MANAGER_H
#define QUADS_MANAGER_H

// To have all quads managed in one space
// Menu Quads
// Level Quads
// Entity quads 
// etc.


namespace qm
{

  void accumulate()

  {
    qm::AllQuads.clear(); // resetting vector

    // assign menu quads
    if(menu::MenuQuads.size() > 0){
      qm::AllQuads.insert(qm::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
    }

        // assign level quads
    if(levels::LevelQuads.size() > 0){
      qm::AllQuads.insert(qm::AllQuads.end(), levels::LevelQuads.begin(), levels::LevelQuads.end());
    }

    // assign text quads
    if(fonts::TextQuads.size() > 0){
      qm::AllQuads.insert(qm::AllQuads.end(), fonts::TextQuads.begin(), fonts::TextQuads.end());
    }

    // quads::print_out_quads(qm::AllQuads);
    qm::QuadsSummary["level"] = levels::LevelQuads.size();
    qm::QuadsSummary["menu"] = menu::MenuQuads.size();
    qm::QuadsSummary["text"] = fonts::TextQuads.size();
  }

  void delete_quad_id(int quad_id)
  {
    //std::cout << "Removing " << quad_id << " from used quads" << std::endl;
    qm::UsedQuadIds.erase(std::remove(qm::UsedQuadIds.begin(), 
                                      qm::UsedQuadIds.end(), quad_id), 
                                      qm::UsedQuadIds.end());

    //std::cout << "Used quads size: " << qm::UsedQuadIds.size() << std::endl;
  }

  void delete_vertex_id(int vertex_id)
  {
    //std::cout << "Removing " << vertex_id << " from used vertexes" << std::endl;
    qm::UsedVertexIds.erase(std::remove(qm::UsedVertexIds.begin(), 
                                        qm::UsedVertexIds.end(), vertex_id), 
                                        qm::UsedVertexIds.end());

    //std::cout << "Used Vertexes size: " << qm::UsedVertexIds.size() << std::endl;
  }



}



  #endif