#ifndef LOGGER_H
#define LOGGER_H

namespace logger
{
  std::string LOG_PATH = "./logs/v2/";
  std::string LOG_PATH_RUN;

  // Logs the message to v2 log file (only one?). Will log all events into one and analyze later. Anyway, better than console
  struct LogMessage
  {
    std::string path;
    std::string module_name;
    std::string file;
    std::string message;
    float time_elapsed;
    int line;
    int level;
    int event_id;

    JS_OBJ(path, module_name, file, message, time_elapsed, line, level, event_id);
  };


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

  // Logs information to logs/v2 logging dir as json objects
  template<typename T>
  void log(int level, int event_id, std::string module_name, std::string file, int line, T message)
  {
    if(level >= LOGGING)
    {
      auto end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end-GAME_START_TIME;
      float elapsed_time = elapsed_seconds.count();
      std::string file_path = LOG_PATH_RUN + "/full.json";

      LogMessage lm;
      lm.file = file;
      lm.path = file_path;
      lm.message = message;
      lm.level = level;
      lm.module_name = module_name;
      lm.line = line;
      lm.time_elapsed = elapsed_time;
      lm.event_id = event_id;

      std::string lm_json_string = JS::serializeStruct(lm);
      std::ofstream json_file (file_path.c_str(), std::ios_base::app);
      if (json_file.is_open())
      {
        // add [ in the beginning
        if(event_id == LOG_EVENT_GAME_START){
            json_file << "[\n";
        }

        for(int i = 0; i < lm_json_string.size(); i ++){
            json_file << lm_json_string[i];
        }

        if(event_id == LOG_EVENT_GAME_EXIT){
          json_file << "]\n";
        } else {
          json_file << ",\n";
        }
        json_file.close();
      }
    }
  }
}

#endif
