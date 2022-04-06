
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "py.h"
#include "../dependencies/pybind11/pybind11.h"
#include "../dependencies/pybind11/embed.h"
#include "../dependencies/pybind11/stl.h"

#include "../modules/anims.h"
#include "../modules/entity.h"
#include "../modules/items.h"
#include "../modules/mobs.h"
#include "../modules/npcs.h"
#include "../modules/travel.h"
#include "../modules/models.h"

namespace py = pybind11;

namespace scripts
{ 
  typedef void (*sig_ptr)();
  std::map<int,sig_ptr> Handler;
  std::map<std::string, float> args;

  py::scoped_interpreter guard{};

  void execute(std::string& script_name)
  {
    using namespace py::literals;
    auto locals = py::dict();
    py::eval_file(script_name, py::globals(), locals);
    scripts::args.clear();

    // Retrieve Function ID
    auto func_id = locals["func_id"].cast<int>();

    // Retrieve dict and assign to external map
    auto vars = locals["vars"].cast<std::map<std::string, float>>();
    args = vars;

    std::cout << "Executing function ID: " << func_id << std::endl;
    scripts::Handler[func_id]();

  };

  void init()
  {
    scripts::Handler[0] = _spawn_mob;
    scripts::Handler[1] = _drop_mob;
    scripts::Handler[2] = _interact_npc;
    scripts::Handler[3] = _spawn_item;
    scripts::Handler[4] = _drop_item;
    scripts::Handler[5] = _start_animation;
    scripts::Handler[6] = _init_travel;
    scripts::Handler[7] = _print_entity;
    scripts::Handler[8] = _print_models;
    scripts::Handler[9] = _print_quads;
    std::cout << "PyScripts Initialized" << std::endl;
  }



  void drop()
  {
    //delete &guard;
  };



  void _spawn_mob()
  {
    // Function 0 -> spawn mob
    mobs::spawn(args["mob_type_id"], args["x"], args["y"]);
  };

  void _drop_mob()
  {
    // Function 1 -> drop mob
    mobs::drop(args["entity_id"]);
  }

  void _interact_npc()
  {
    // Function 2 -> interact_npc
    npcs::interact(args["entity_id"], args["interaction_value"]);
  }

  void _spawn_item()
  {
    // Function 3 -> spawn_item
    items::spawn(args["item_id"], args["x"], args["y"]);
  }

  void _drop_item()
  {
    // Function 4 -> _drop_item
    items::drop(args["entity_id"]);
  }

  void _start_animation()
  {
    // Function 5 -> _start_animation
    anims::start(args["anim_type_id"], args["entity_id"]);
  }

  void _init_travel()
  {
    // Function 6 -> _init_travel
    travel::init_travel_with_target(args["entity_id"], args["target_x"], args["target_y"]);
  }

  void _print_entity()
  {
    // Function 7 -> _print_entity
    entity::print_entity_data();
  }


  void _print_models()
  {
    // Function 8 -> _print_models
    models::print_models_data();
  }

  void _print_quads()
  {
    // Function 9 -> _print_quads
    quads::print_quads_data();
  }


}

