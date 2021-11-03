#include <string>
#include <vector>
#include <dirent.h>
#include "utils.h"
#include <fstream>
#include <sstream>
#include <random>
#include "../dependencies/json_struct.h"

// Random reused utility functions
namespace utils2
{
  std::string read_text_file(std::string path)
  {
    std::ifstream json_file(path);
    std::ostringstream tmp;
    tmp << json_file.rdbuf();
    std::string s = tmp.str();
    return s;
  };

  bool has_ending(std::string const &full_string, std::string const &ending) 
  {
    if (full_string.length() >= ending.length()) {
        return (0 == full_string.compare (full_string.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
  };

  std::vector<std::string> list_json_files(std::string path)
  {
    std::vector<std::string> list_of_files = {};
    struct dirent *entry;
    DIR *dir = opendir(path.c_str());
    while ((entry = readdir(dir)) != NULL) {
      if(has_ending (entry->d_name, ".json")){
        std::string str = entry->d_name;
        std::string clean_str = str.replace(str.end()-5, str.end(), "");
        list_of_files.push_back(clean_str);  
      }
    }
    closedir(dir);
    return list_of_files;
  };  

  std::vector<std::string> list_any_files(std::string path)
  {
    std::vector<std::string> list_of_files = {};
    struct dirent *entry;
    DIR *dir = opendir(path.c_str());
    while ((entry = readdir(dir)) != NULL)
    {
      std::string str = entry->d_name;
      list_of_files.push_back(entry->d_name );  
    }
    closedir(dir);
    return list_of_files;
  };  

  // void check_glew(GLenum err)
  // {
  //   if (err != GLEW_OK)
  //   {
  //       exit(1); 
  //   }
  // // check that the machine supports the 2.1 API.
  //   if (!GLEW_VERSION_2_1)
  //   { 
  //     exit(1); 
  //   }
  // }

  int get_random(int min_int, int max_int)
  {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min_int, max_int); // define the range
    int rand_num = distr(gen);
    return rand_num;
  }

}
