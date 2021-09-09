#ifndef LOGGER_H
#define LOGGER_H

namespace logger
{
  std::string LOG_PATH = "./logs/v2/";
  std::string LOG_PATH_RUN;

  void init()
  {
    // get today date in yyyymmdd
    const int MAXLEN = 80;
    char s[MAXLEN];
    time_t t = time(0);
    strftime(s, MAXLEN, "%Y%m%d", localtime(&t));
    std::string today_date = s;

    // if not found, create new directory for todays logs:
    if(!(std::filesystem::exists(logger::LOG_PATH+today_date)))
    {
      std::string new_dir = logger::LOG_PATH+today_date;
      std::string new_dir_run = logger::LOG_PATH+today_date+"/0";
      std::filesystem::create_directory(new_dir.c_str());
      std::filesystem::create_directory(new_dir_run.c_str());
      LOG_PATH_RUN = new_dir_run;
    } else {
      // directory exists - lets find new run number
      std::vector<std::string> run_list = utils::list_any_files(logger::LOG_PATH+today_date);
      std::string max_run_id = *max_element(run_list.begin(), run_list.end());
      int new_run_id = stoi(max_run_id) + 1;
      std::string dir_run = logger::LOG_PATH+today_date+"/"+std::to_string(new_run_id);
      std::filesystem::create_directory(dir_run);
      LOG_PATH_RUN = dir_run;
    }


  }




  // debug 0 
  // info  1
  // error 2

  template<typename T>
  void print(T message, int level=1)
  {
    if(level >= LOGGING)
    {
      std::cout << message << std::endl;
    }
  }

  template<typename T>
  void print_vector(std::vector<T> v, std::string message="")
  {
    for(int i=0; i<v.size(); i++)
    {
      std::cout << message << " id: " << i << " value: " << v[i] << std::endl;
    }
  }

  // need something that gives [log level - module - funtion - file - time - message information]

  template<typename T>
  void log(int level, std::string module, std::string file, int line, T message)
  {
    if(level >= LOGGING)
    {
      std::string log_path = LOG_PATH + module;


    }
  }

}






#endif