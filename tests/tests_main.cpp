

#include <iostream>
#include <map>
#include <string>
#include <vector>
// #include "catch.hpp"

#include "tests_utils.h"

typedef bool (*sig_ptr)();
std::map<int,sig_ptr> tests;
float total_tests;
float passed;
float failed;
std::vector<int> failed_tests;


void init()
{
  tests[0] = test_utils_generate_id_first_id_is_1;
  tests[1] = test_utils_drop_id_empty_index;
  tests[2] = test_utils_re_generate_ids;
};


void eval(int test_id, bool test_result)
{
  // IF true (passed)
  if(test_result)
  {
    std::cout << "Test ID " << test_id << " name: " << tests[test_id] << " Passed!" << std::endl;
    passed += 1;
  }
  else
  {
    std::cout << "Test ID " << test_id << " Failed!" << std::endl;
    failed += 1;
    failed_tests.push_back(test_id);
  }
};

int main()
{
  init();
  std::cout << "Starting tests..." << std::endl;
  std::cout << std::endl;

  for (auto const& [k, v]:tests)
  {
    total_tests += 1;
    bool test_result = v();
    eval(k, test_result);
  }
  std::cout << std::endl;
  std::cout << "Tests finished" << std::endl;
  std::cout << std::endl;
  std::cout << "======== SUMMARY ========" << std::endl;
  std::cout << "  Total tests: " << total_tests << std::endl;
  std::cout << "    Passed: " << passed << " (" << passed/total_tests << ")" <<std::endl;
  std::cout << "    Failed: " << failed << " (" << failed/total_tests << ")" <<std::endl;
  std::cout << std::endl;
  if(failed>0)
  {
    std::cout << "    Failed test IDS: ";
    for(int f=0; f<failed_tests.size(); f++)
    {
      std::cout << failed_tests[f] << ",";
    }
    std::cout << std::endl;
  }
  return 0;
}

