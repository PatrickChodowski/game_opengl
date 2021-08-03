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
    qm::AllQuads.clear(); // resetting map

    // assign level quads
    if(levels::LevelQuads.size() > 0){
      qm::AllQuads.insert(qm::AllQuads.end(), levels::LevelQuads.begin(), levels::LevelQuads.end());
    }

    // assign menu quads
    if(menu::MenuQuads.size() > 0){
      qm::AllQuads.insert(qm::AllQuads.end(), menu::MenuQuads.begin(), menu::MenuQuads.end());
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


  int gen_quad_id()
  {
    // is supposed to return new quad id based on qm.next_quad_id
    // yes, kinda updates namespace's global variable
    qm::next_quad_id += 1;
    // logger::print("Next Quad ID: "+std::to_string(qm::next_quad_id));
    return qm::next_quad_id;
  }











}



  #endif