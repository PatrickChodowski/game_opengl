#include <string>
#include <vector>

#include <GL/glew.h> 

#ifndef MODULES_UTILS_H
#define MODULES_UTILS_H

namespace utils2
{
  // Reads in the text file
  std::string read_text_file(std::string path);

  // checks if string has specified ending
  bool has_ending(std::string const &full_string, std::string const &ending);

  // list all json files in directory that have metadata created, default path
  std::vector<std::string> list_json_files(std::string path);

  // List all files in the directory
  std::vector<std::string> list_any_files(std::string path);

  // Checks glew
  void check_glew(GLenum err);

  // Get random int from the range
  int get_random(int min_int, int max_int);
}

#endif
