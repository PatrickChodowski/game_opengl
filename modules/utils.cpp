#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <dirent.h>
#include "utils.h"

#include "../dependencies/json_struct.h"
#include <GL/glew.h> 


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

  void check_glew(GLenum err)
  {
    if (err != GLEW_OK)
    {
        exit(1); 
    }
  // check that the machine supports the 2.1 API.
    if (!GLEW_VERSION_2_1)
    { 
      exit(1); 
    }
  }

  int get_random(int min_int, int max_int)
  {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min_int, max_int); // define the range
    int rand_num = distr(gen);
    return rand_num;
  }


  void array_to_file(std::string file_name, float* arr, int array_size, int new_line_count)
  {
    std::string file_path = "./logs/"+file_name + ".txt";
    std::ofstream array_file (file_path.c_str());
    if (array_file.is_open())
    {
      for(int i = 0; i < array_size; i ++){
          array_file << arr[i] << " " ;
          if(((i+1) % new_line_count == 0) && (i>0)){
            array_file << "\n";
          }
      }
      array_file.close();
    }
  };

  int _find_next_id(std::vector<int>& used_ids_table)
  {
    int n = used_ids_table.size();
    // for whole vector, find value that would be bigger than (index + 1)
    for (int i = 0; i < n; i++)
    {
      if (used_ids_table[i] > (i+1)){
        return i+1;
      }
    }
    return n+1;
  }

  int generate_id(std::vector<int>& used_ids_table)
  {
    int next_id = utils2::_find_next_id(used_ids_table);
    used_ids_table.push_back(next_id);
    return next_id;
  };

  void drop_id(std::vector<int>& used_ids_table, int id)
  {
    used_ids_table.erase(std::remove(used_ids_table.begin(), used_ids_table.end(), id), used_ids_table.end());
  };

  template <typename T>
  std::string str(T number)
  {
    return std::to_string(number);
  }

  static void GlClearError()
  {
    while(glGetError() != GL_NO_ERROR);
  };

  static bool GlLogCall(const char* function, const char* file, int line)
  { 
    while(GLenum error = glGetError())
    {
      std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << " " << line << std::endl;
      return false;
    }
    return true;
  };


  template std::string utils2::str<int>(int);
  template std::string utils2::str<float>(float);

}
