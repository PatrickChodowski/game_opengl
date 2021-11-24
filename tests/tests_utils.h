
#include "../modules/utils.h"

bool test_utils_generate_id_first_id_is_1()
{
  bool passed = false;
  std::vector<int> Index = {};
  int next_id = utils2::generate_id(Index);

  if(next_id == 1)
  {
    passed = true;
  }
  return passed;
}


bool test_utils_drop_id_empty_index()
{
  bool passed = false;
  std::vector<int> Index = {};
  int next_id = utils2::generate_id(Index);
  utils2::drop_id(Index, next_id);

  if(Index.size() == 0)
  {
    passed = true;
  }
  return passed;
}


bool test_utils_re_generate_ids()
{
  bool passed = false;
  std::vector<int> Index = {};
  int id1 = utils2::generate_id(Index);
  int id2 = utils2::generate_id(Index);
  int id3 = utils2::generate_id(Index);
  int id4 = utils2::generate_id(Index);
  utils2::drop_id(Index, 1);
  utils2::drop_id(Index, 2);

  if(Index.size() == 0)
  {
    passed = true;
  }
  return passed;
}