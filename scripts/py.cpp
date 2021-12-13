
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "py.h"
#include "../dependencies/pybind11/pybind11.h"
#include "../dependencies/pybind11/embed.h"
#include "../dependencies/pybind11/stl.h"

#include "../modules/entity.h"
#include "../modules/mobs.h"
#include "../modules/npcs.h"

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


}

