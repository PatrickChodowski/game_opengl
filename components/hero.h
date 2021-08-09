#ifndef HERO_H
#define HERO_H

/// how to render hero and also how to update only the correct part of the buffer?
// will be a bluebrint for other renderable entities
namespace hero
{

  float HERO_X = ((float)WINDOW_WIDTH)/2 - (70/2);
  float HERO_Y = ((float)WINDOW_HEIGHT)/2 - (90/2);
  int current_frame = 0;
  float frame_delay = 0.2;
  auto frame_update_time = std::chrono::system_clock::now();


  struct Hero
  {
    // position
    int x;
    int y;
    int map_id;

    // display information
    int current_movement_state;
    
    // personal information
    std::string name;
    std::string type;
    int texture_id;
    int level;

    // statistics
    int exp;
    int speed;
    int hp;
    int dmg;
    int def;
    int mobs_killed;

    JS_OBJ(x, y, map_id, name, type, texture_id, level, exp, speed, hp, dmg, def, mobs_killed);
  }; 

  Hero load_from_save(std::string save_name)
  {

    Hero hero;
    std::string file_path = "saves/" + save_name + ".json";
    std::string json_data = utils::read_text_file(file_path);
    JS::ParseContext context(json_data);
    context.parseTo(hero);

    if(LOGGING == 0)
    {
      std::cout << "Loaded Hero from save: " << save_name << std::endl
      << " name: " << hero.name << " type: " << hero.type << std::endl
      << " position: " << hero.x << "," << hero.y << " map_id: " << hero.map_id << std::endl
      << " texture_id: " << hero.texture_id << " experience: " << hero.exp << std::endl
      << " hp: " << hero.hp << " speed: " << hero.speed << " dmg: " << hero.dmg << std::endl
      << " def: " << hero.def << " mobs killed: " << hero.mobs_killed << std::endl;
    }

    return hero;
  }


  void update_frame(int event_id)
  {
    // camera movement will update the hero frame_id (updated in events.h -> handle_game_on_controls)
    // logic based on current_frame_id and new_frame_id
    // basic frame_id -> 0
    // events:
    // #define MOVE_LEFT 0
    // #define MOVE_RIGHT 1
    // #define MOVE_UP 2
    // #define MOVE_DOWN 3
    // #define STAND_STILL 4
    // #define ATTACK 5
    float time_since_last_update = timer::get_elapsed_time(hero::frame_update_time);
    if(time_since_last_update >= hero::frame_delay){
      switch(current_frame)
      {
        // standing still
        case 0: 
        case 1:
        case 2:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 10; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;

        // facing left
        case 7:
          switch(event_id)
          {
            case 0: current_frame = 6; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;

        // facing left
        case 6:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;

        // facing right
        case 9:
          switch(event_id)
          {
            case 0: current_frame = 6; break;
            case 1: current_frame = 10; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;

        // facing right
        case 10:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;

        // facing down
        case 3:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 4; break;
          }
          break;
        // facing down
        case 4:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;
        // facing up
        case 11:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 12; break;
            case 3: current_frame = 4; break;
          }
          break;
        // facing up
        case 12:
          switch(event_id)
          {
            case 0: current_frame = 7; break;
            case 1: current_frame = 9; break;
            case 2: current_frame = 11; break;
            case 3: current_frame = 3; break;
          }
          break;
      } 
      hero::frame_update_time = std::chrono::system_clock::now();
    }
  }
}



#endif