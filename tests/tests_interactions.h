
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../modules/interactions.h"
#include <vector>

bool test_interactions__record_interaction()
{
  bool passed = false;
  interactions::init();
  interactions::record(500);
  if(interactions::did_it_happen(500)){
    passed = true;
  }
  return passed;
}

bool test_interactions__clear_interaction()
{
  bool passed = false;
  interactions::init();
  interactions::record(500);
  interactions::clear();
  if(!interactions::did_it_happen(500)){
    passed = true;
  }
  return passed;
}

bool test_interactions__check_different()
{
  bool passed = false;
  interactions::init();
  interactions::record(500);
  if(!interactions::did_it_happen(600)){
    passed = true;
  }
  return passed;
}

