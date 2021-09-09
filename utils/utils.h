#ifndef UTILS_H
#define UTILS_H

namespace utils
{


  // Reading in text files (may be moved later to utils if needed)
  std::string read_text_file(std::string path)
  {
    std::ifstream json_file(path);
    std::ostringstream tmp;
    tmp << json_file.rdbuf();
    std::string s = tmp.str();
    return s;
  } 

  bool has_ending (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
  }

 // list all json files in directory that have metadata created, default path
  std::vector<std::string> list_files(std::string path)
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

    if (err != GLEW_OK){
        exit(1); // or handle the error in a nicer way
    }

    if (!GLEW_VERSION_2_1){  // check that the machine supports the 2.1 API.
      exit(1); // or handle the error in a nicer way
    }
  }

  template<typename T>
  void array_to_file(std::string file_name, T *array_pointer, int array_size, int new_line_count){
    std::string file_path = "./logs/"+file_name + ".txt";
    std::ofstream array_file (file_path.c_str());
    if (array_file.is_open())
    {
      for(int i = 0; i < array_size; i ++){
          array_file << array_pointer[i] << " " ;
          if(((i+1) % new_line_count == 0) && (i>0)){
            array_file << "\n";
          }
      }
      array_file.close();
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

}
#endif