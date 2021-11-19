
#include "logger.h"
#include "game.h"
#include "quads.h"
#include "textures.h"
#include "timer.h"
#include "utils.h"

#include <chrono>
#include <ctime> 
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../dictionary.h"

namespace logger
{
  std::string LOG_PATH_RUN;
  std::string LOG_PATH = "./logs/v2/";
  std::chrono::time_point<std::chrono::high_resolution_clock> function_start_time;

  int LOG_LEVEL = LOG_LVL_INFO;

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
      std::vector<std::string> run_list = utils2::list_any_files(logger::LOG_PATH+today_date);
      std::vector<int> run_list_int = {};
      for(int r=0; r<run_list.size(); r++)
      {
        if( std::isdigit(run_list[r][0]) ){
          run_list_int.push_back(stoi(run_list[r]));
        }
      }

      int max_run_id = *max_element(run_list_int.begin(), run_list_int.end());
      int new_run_id = max_run_id + 1;

      std::string dir_run = logger::LOG_PATH+today_date+"/"+std::to_string(new_run_id);
      std::filesystem::create_directory(dir_run);
      LOG_PATH_RUN = dir_run;
    }
  }

  template<typename T>
  void print(T message)
  {
    std::cout << message << std::endl;
  };


  void log(int log_level, 
           std::string msg,
           std::string module,
           std::string file,
           int line,
           int type
           )
  {
    // Log only if the information is greater or equal to set log level
    if(log_level >= logger::LOG_LEVEL)
    {
      float fte;
      if(type == LOG_START_TIMER)
      {
        logger::function_start_time = std::chrono::high_resolution_clock::now();
        fte = 0;
      } else if (type == LOG_END_TIMER)
      {
        fte = timer2::get_elapsed_time(logger::function_start_time);
      }

      float time_since_game_start = timer2::get_elapsed_time(game2::GAME_START_TIME);
      std::string file_path = LOG_PATH_RUN + "/full.json";

      logger::print(msg);

      LogMessage lm;
      lm.file = file;
      lm.path = file_path;
      lm.msg = msg;
      lm.log_level = log_level;
      lm.module = module;
      lm.line = line;
      lm.time_since_game_start = time_since_game_start;
      lm.function_time_elapsed = fte;
      lm.type = type;
      lm.current_time = timer2::get_current_time();

      std::string lm_json_string = JS::serializeStruct(lm);
      std::ofstream json_file (file_path.c_str(), std::ios_base::app);
      if (json_file.is_open())
      {
        if(type == LOG_GAME_START)
        {
          json_file << "[\n";
        }

        for(int i = 0; i < lm_json_string.size(); i ++)
        {
          json_file << lm_json_string[i];
        }

        if(type == LOG_GAME_END){
          json_file << "]\n";
        } else {
          json_file << ",\n";
        }
        json_file.close();
      }
    }
  };



  void log_data()
  {
    // quads2::log();
    // textures2::log();
  };

}
