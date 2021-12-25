
#include "../modules/utils.h"
#include "../dependencies/parallel_hashmap/phmap.h"

bool test_utils_generate_id_first_id_is_1()
{
  bool passed = false;
  std::vector<int> Index = {};
  int next_id = utils::generate_id(Index);

  if(next_id == 1){passed = true;}
  return passed;
}

bool test_utils_drop_id_empty_index()
{
  bool passed = false;
  std::vector<int> Index = {};
  int next_id = utils::generate_id(Index);
  utils::drop_id(Index, next_id);

  if(Index.size() == 0){passed = true;}
  return passed;
}

bool test_utils_re_generate_ids()
{
  bool passed = false;
  std::vector<int> Index = {};
  int id1 = utils::generate_id(Index);
  int id2 = utils::generate_id(Index);
  int id3 = utils::generate_id(Index);
  int id4 = utils::generate_id(Index);
  utils::drop_id(Index, 1);
  utils::drop_id(Index, 2);
  int id5 = utils::generate_id(Index);

  if(id5 == 1){passed = true;}
  return passed;
}

bool test_utils_re_generate_ids2()
{
  bool passed = false;
  std::vector<int> Index = {};
  int id1 = utils::generate_id(Index);
  int id2 = utils::generate_id(Index);
  int id3 = utils::generate_id(Index);
  int id4 = utils::generate_id(Index);
  utils::drop_id(Index, 1);
  utils::drop_id(Index, 2);
  int id5 = utils::generate_id(Index);
  int id6 = utils::generate_id(Index);

  if(id6 == 2){passed = true;}
  return passed;
}

bool test_utils_re_generate_ids3()
{
  bool passed = false;
  std::vector<int> Index = {};
  int id1 = utils::generate_id(Index);
  int id2 = utils::generate_id(Index);
  int id3 = utils::generate_id(Index);
  int id4 = utils::generate_id(Index);
  utils::drop_id(Index, 1);
  utils::drop_id(Index, 2);
  int id5 = utils::generate_id(Index);
  int id6 = utils::generate_id(Index);
  int id7 = utils::generate_id(Index);

  if(id7 == 5){passed = true;}
  return passed;
}