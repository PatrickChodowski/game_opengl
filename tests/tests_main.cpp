

#include <iostream>
#include <string>
#include <vector>
#include "../dependencies/parallel_hashmap/phmap.h"

#include "tests_collisions.h"
#include "tests_models.h"
#include "tests_utils.h"

typedef bool (*sig_ptr)();
phmap::flat_hash_map<std::string, sig_ptr> tests;
float total_tests;
float passed;
float failed;
std::vector<std::string> failed_tests;


void init()
{
  // collisions
  tests["test_collisions__set_sensors_on_entity_count"] = test_collisions__set_sensors_on_entity_count;
  tests["test_collisions__set_sensors_on_entity_top"] = test_collisions__set_sensors_on_entity_top;
  tests["test_collisions__set_sensors_on_entity_bottomleft"] = test_collisions__set_sensors_on_entity_bottomleft;
  tests["test_collisions__set_sensors_on_entity_count_after_update"] = test_collisions__set_sensors_on_entity_count_after_update;
  tests["test_collisions__set_sensors_on_entity_top_after_update"] = test_collisions__set_sensors_on_entity_top_after_update;
  tests["test_collisions__get_entity_to_map_dto_solidpass"] = test_collisions__get_entity_to_map_dto_solidpass;
  tests["test_collisions__get_entity_to_map_dto_is_near"] = test_collisions__get_entity_to_map_dto_is_near;
  // tests["test_collisions__get_entity_to_entity_dto_solidpass"] = test_collisions__get_entity_to_entity_dto_solidpass;

  
  // models
  // tests["test_models_float_to_bytes_conversion"] = test_models_float_to_bytes_conversion;
  tests["test_models_bytes_to_float_conversion_05"] = test_models_bytes_to_float_conversion_05;
  tests["test_models_bytes_to_float_conversion_51"] = test_models_bytes_to_float_conversion_51;


  // utils
  tests["test_utils_generate_id_first_id_is_1"] = test_utils_generate_id_first_id_is_1;
  tests["test_utils_drop_id_empty_index"] = test_utils_drop_id_empty_index;
  tests["test_utils_re_generate_ids"] = test_utils_re_generate_ids;
  tests["test_utils_re_generate_ids2"] = test_utils_re_generate_ids2;
  tests["test_utils_re_generate_ids3"] = test_utils_re_generate_ids3;
};


void eval(std::string test_name, bool test_result)
{
  if(test_result)
  {
    std::cout << " - \033[1;32mTest \033[0m" << test_name << " \033[1;32m Passed! \033[0m" << std::endl;
    passed += 1;
  }
  else
  {
    std::cout << " - \033[1;31mTest \033[0m" << test_name << " \033[1;31m Failed! \033[0m" << std::endl;
    failed += 1;
    failed_tests.push_back(test_name);
  }
};

int main()
{
  init();
  std::cout << std::endl;
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
      std::cout << failed_tests[f] << ", ";
    }
    std::cout << std::endl;
  }
  return 0;
}

