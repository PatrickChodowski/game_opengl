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



}



  #endif