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
        logger::print(clean_str);
        list_of_files.push_back(clean_str);  
      }
    }
    closedir(dir);
    return list_of_files;
  };  


}


#endif