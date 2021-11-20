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

  // Float array saved to file
  void array_to_file(std::string file_name, float* arr, int array_size, int new_line_count);

  //  Find next available ID for provided IndexTable
  int _find_next_id(std::vector<int>& used_ids_table);

  //  Generate (find and insert) next available ID for provided IndexTable
  int generate_id(std::vector<int>& used_ids_table);

  //  Drop ID from provided IndexTable
  void drop_id(std::vector<int>& used_ids_table, int id);

  #define ASSERT(x) if(!(x)) raise(SIGTRAP);
  #define GlCall(x) GlClearError();x;ASSERT(GlLogCall(#x, __FILE__, __LINE__));

}

#endif
