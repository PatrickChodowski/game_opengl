#ifndef HERO_H
#define HERO_H


/// how to render hero and also how to update only the correct part of the buffer?
// will be a bluebrint for other renderable entities
namespace hero
{
  float HERO_WIDTH = 70.0;
  float HERO_HEIGHT = 90.0;
  float HERO_X = ((float)WINDOW_WIDTH)/2 - (HERO_WIDTH/2);
  float HERO_Y = ((float)WINDOW_HEIGHT)/2 - (HERO_HEIGHT/2);
  int current_frame = 0;
  auto frame_update_time = std::chrono::system_clock::now();
  bool attack_state = false;


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
  void set_current_frame(int frame_id)
  {
    hero::current_frame = frame_id;
    hero::frame_update_time = std::chrono::system_clock::now();
  }

  void update_frame(int event_id)
  {
    float time_since_last_update = timer::get_elapsed_time(hero::frame_update_time);
    // if(event_id == MOVE_LEFT){
    //   if(current_frame == FRAME_LEFT_1 && time_since_last_update >= FRAME_DELAY)
    //   {
    //     hero::set_current_frame(FRAME_LEFT_3);
    //   } else if(current_frame == FRAME_LEFT_3 && time_since_last_update >= FRAME_DELAY)
    //   {
    //     hero::set_current_frame(FRAME_LEFT_1);
    //   } else if (time_since_last_update >= FRAME_DELAY_SHORT && 
    //             current_frame != FRAME_LEFT_3 && 
    //             current_frame != FRAME_LEFT_1)
    //   {
    //     hero::set_current_frame(FRAME_LEFT_1);
    //   }
    // } 
     if(event_id == MOVE_RIGHT){
      if(current_frame == FRAME_RIGHT_1 && time_since_last_update >= FRAME_DELAY)
      {
        hero::set_current_frame(FRAME_RIGHT_3);
      } else if(current_frame == FRAME_RIGHT_3 && time_since_last_update >= FRAME_DELAY)
      {
        hero::set_current_frame(FRAME_RIGHT_1);

      } else if (time_since_last_update >= FRAME_DELAY_SHORT && 
                current_frame != FRAME_RIGHT_3 && 
                current_frame != FRAME_RIGHT_1)
      {
        hero::set_current_frame(FRAME_RIGHT_1);
      }
    } else if(event_id == MOVE_UP){
      if(current_frame == FRAME_UP_1 && time_since_last_update >= FRAME_DELAY)
      {
        hero::set_current_frame(FRAME_UP_2);
      } else if(current_frame == FRAME_UP_2 && time_since_last_update >= FRAME_DELAY)
      {
        hero::set_current_frame(FRAME_UP_1);
      } else if (time_since_last_update >= FRAME_DELAY_SHORT &&
                current_frame != FRAME_UP_2 && 
                current_frame != FRAME_UP_1)
      {
        hero::set_current_frame(FRAME_UP_1);
      }
    } else if(event_id == MOVE_DOWN){
      if(current_frame == FRAME_DOWN_1 && time_since_last_update >= FRAME_DELAY)
      {
         hero::set_current_frame(FRAME_DOWN_2);
      } else if(current_frame == FRAME_DOWN_2 && time_since_last_update >= FRAME_DELAY)
      {
         hero::set_current_frame(FRAME_DOWN_1);
      } else if (time_since_last_update >= FRAME_DELAY_SHORT &&
                current_frame != FRAME_DOWN_2 && 
                current_frame != FRAME_DOWN_1)
      {
        hero::set_current_frame(FRAME_DOWN_1);
      }
    } else if (event_id == STAND_STILL){
      if(current_frame == FRAME_STAND_1 && time_since_last_update >= FRAME_DELAY_LONG){
        hero::set_current_frame(FRAME_STAND_2);
      } else if (current_frame == FRAME_STAND_2 && time_since_last_update >= FRAME_DELAY_SHORT){
        hero::set_current_frame(FRAME_STAND_3);
      } else if (current_frame == FRAME_STAND_3 && time_since_last_update >= FRAME_DELAY){
        hero::set_current_frame(FRAME_STAND_1);
      } else if (time_since_last_update >= FRAME_DELAY &&
                current_frame != FRAME_STAND_1 && 
                current_frame != FRAME_STAND_2 &&
                current_frame != FRAME_STAND_3){
        hero::set_current_frame(FRAME_STAND_1);
      }
    } else if (event_id == ATTACK){
      if(current_frame == FRAME_ATTACK_1 && time_since_last_update >= FRAME_DELAY_SHORT){
        hero::set_current_frame(FRAME_ATTACK_2);
      } else if(current_frame == FRAME_ATTACK_2 && time_since_last_update >= FRAME_DELAY_SHORT){
        hero::set_current_frame(FRAME_ATTACK_3);
        hero::attack_state = false;
      } else if(current_frame == FRAME_ATTACK_3 && time_since_last_update >= FRAME_DELAY_SHORT){
        hero::set_current_frame(FRAME_ATTACK_1);
      } else if(current_frame != FRAME_ATTACK_3 && 
                current_frame != FRAME_ATTACK_2 &&
                current_frame != FRAME_ATTACK_1 &&
                time_since_last_update >= FRAME_DELAY_SHORT){
        hero::set_current_frame(FRAME_ATTACK_1);
      }
    }
  }

  // hero's frame should also just pick the item frame that hero is holding
  // heeheee sure
  int get_hero_quad_id()
  {
    // find quad id of the hero
    int hero_quad_id = -1;
    for(int e = 0; e < ent::EntityQuads.size(); e++)
    {
      if(ent::EntityQuads[e].entity_type_id == 0){
        hero_quad_id = ent::EntityQuads[e].id;
        break;
      }
    }
    return hero_quad_id;
  }


  void set_hero_sensors()
  {
    int hero_quad_id = hero::get_hero_quad_id();
    int hid = quads::find_quad_id(hero_quad_id, quads::AllQuads);
    quads::AllQuads[hid].sensors.clear();
    float hero_center_x = quads::AllQuads[hid].s_x + ((quads::AllQuads[hid].s_w)/2);
    float hero_center_y = quads::AllQuads[hid].s_y + ((quads::AllQuads[hid].s_h)/2);

    for(int i=0; i < colls::SENSOR_COUNT; i++)
    {
      colls::Sensor s;
      switch(i) {
        case SENSOR_TOP:
            s.x = hero_center_x;
            s.y = (quads::AllQuads[hid].s_y - colls::SENSOR_OFFSET);
            s.id = SENSOR_TOP;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_TOP, s));
        break;
        case SENSOR_TOP_RIGHT:
            s.x = (quads::AllQuads[hid].s_x + quads::AllQuads[hid].s_w + colls::SENSOR_OFFSET);
            s.y = (quads::AllQuads[hid].s_y  - colls::SENSOR_OFFSET);
            s.id = SENSOR_TOP_RIGHT;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_TOP_RIGHT, s));
        break;
        case SENSOR_RIGHT:
            s.x = (quads::AllQuads[hid].s_x + quads::AllQuads[hid].s_w  + colls::SENSOR_OFFSET);
            s.y = (hero_center_y) ;
            s.id = SENSOR_RIGHT;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_RIGHT, s));
        break;
        case SENSOR_BOTTOM_RIGHT: // bottom right
            s.x = (quads::AllQuads[hid].s_x + quads::AllQuads[hid].s_w + colls::SENSOR_OFFSET);
            s.y = (quads::AllQuads[hid].s_y + quads::AllQuads[hid].s_h + colls::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_RIGHT;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_BOTTOM_RIGHT, s));
        break;
        case SENSOR_BOTTOM:
            s.x = hero_center_x;
            s.y = (quads::AllQuads[hid].s_y + quads::AllQuads[hid].s_h + colls::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_BOTTOM, s));
        break;
        case SENSOR_BOTTOM_LEFT:
            s.x = quads::AllQuads[hid].s_x - colls::SENSOR_OFFSET;
            s.y = (quads::AllQuads[hid].s_y + quads::AllQuads[hid].s_h + colls::SENSOR_OFFSET);
            s.id = SENSOR_BOTTOM_LEFT;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_BOTTOM_LEFT, s));
        break;
        case SENSOR_LEFT:
            s.x = quads::AllQuads[hid].s_x - colls::SENSOR_OFFSET ;
            s.y = hero_center_y;
            s.id = SENSOR_LEFT;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_LEFT, s));
        break;
        case SENSOR_TOP_LEFT:
            s.x = quads::AllQuads[hid].s_x - colls::SENSOR_OFFSET;
            s.y = quads::AllQuads[hid].s_y - colls::SENSOR_OFFSET;
            s.id = SENSOR_TOP_LEFT;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_TOP_LEFT, s));
        break;
        case SENSOR_CENTER:
            s.x = hero_center_x;
            s.y = hero_center_y;
            s.id = SENSOR_CENTER;
            quads::AllQuads[hid].sensors.insert(std::pair<int, colls::Sensor>(SENSOR_CENTER, s));
        break;
      }
    }
  }
}




#endif