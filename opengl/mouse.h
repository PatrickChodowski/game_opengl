#ifndef MOUSE_H
#define MOUSE_H


namespace mouse
{
  // temporarily here?
  void click_quad(int clicked_quad_id)
  {
    int quad_index = quads::find_quad_id(clicked_quad_id, quads::AllQuads);
    // logger::print("Quad index: " + std::to_string(quad_index));
    if(quad_index != -1){
      int quad_type_id = quads::AllQuads[quad_index].type_id;

      logger::print("Clicking on " + std::to_string(clicked_quad_id));
      logger::print("Is clicked " + std::to_string(quads::AllQuads[quad_index].is_clicked));
      logger::print("Double check quad id:" + std::to_string(quads::AllQuads[quad_index].id));
      logger::print("Quad Type ID: " + std::to_string(quad_type_id));
      logger::print("is solid?: " + std::to_string(quads::AllQuads[quad_index].solid));
      logger::print("is coll?: " + std::to_string(quads::AllQuads[quad_index].coll));

      quads::click(clicked_quad_id, quad_type_id);

      // std::cout << "Clicked button: " << menu::CurrentMenuButtons[clicked_quad_id] << std::endl;
      if(menu::CurrentMenuButtons[clicked_quad_id] == "main_new")
      {
        game::set_state("NEW_GAME_MENU");
      } else if (menu::CurrentMenuButtons[clicked_quad_id] == "new_game_name")
      {
        if(menu::validate_name()){
          CAMPAIGN_NAME = menu::NewGameName;
          NEW_GAME = true;
          game::set_state("GAME_ON");
        }
      }
      else if (menu::CurrentMenuButtons[clicked_quad_id] == "back_to_main")
      {
        game::set_state("MAIN_MENU");
      }
      else if (menu::CurrentMenuButtons[clicked_quad_id] == "main_load")
      {
        game::set_state("LOAD_GAME_MENU");
      }
      else if (menu::CurrentMenuButtons[clicked_quad_id] == "load_save")
      {
        CAMPAIGN_NAME = menu::LoadSaveButtons[clicked_quad_id];
        NEW_GAME = false;
        game::set_state("GAME_ON");
      }
      else if (menu::CurrentMenuButtons[clicked_quad_id] == "main_exit")
      {
        RUNNING = false;
      }
    }
  }


  void query_quads(int x, int y, std::vector<quads::Quad> quads)
  {
    int quad_id;
    for(auto q : quads)
    {
      if((q.s_x <= x) & (x < q.s_x+q.s_w) & (y >= q.s_y & y < q.s_y + q.s_h))
      {
          logger::print("Clicked tile_id: " + std::to_string(q.id)); 
          click_quad(q.id);
      }
    };
  }


  struct Mouse
  {
    int x;
    int y;

    int last_click_timestamp;
    bool was_released;
  };

  void print_mouse(SDL_MouseMotionEvent e, std::string  name)
  {
    std::cout << "Click pos: "  << e.x << "," << e.y 
              << " timestamp:  "     << e.timestamp 
              << " Which: " << name
              << std::endl;
  }

  void handle_mouse(SDL_MouseMotionEvent e,  
                    SDL_MouseButtonEvent b, 
                    std::vector<quads::Quad> quads)
  {
    switch (b.button)
    {
      case SDL_BUTTON_LEFT:
        print_mouse(e, "Left");
        query_quads(e.x, e.y, quads);
        // mobs::move_to_point(160, e.x, e.y);

        mobs::switch_aggro();
        // mobs::move_to_point(e.x, e.y);
        mobs::move_aggro_mobs_to_point(e.x, e.y);
      break;

      case SDL_BUTTON_RIGHT:
        print_mouse(e, "Right");
      break;

      case SDL_BUTTON_MIDDLE:
        print_mouse(e, "Middle");
      break;
    }
  }
}

#endif
