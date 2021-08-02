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
    // logger::print("Next Quad ID: "+std::to_string(qm::next_quad_id));
    return qm::next_quad_id;
  }











}



  #endif