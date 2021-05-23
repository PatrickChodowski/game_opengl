#ifndef TEXTURES_H
#define TEXTURES_H


namespace textures
{
  struct Frame
  {
    std::string type; //object, tile, entity?
    int x;
    int y;
    int w;
    int h;
  };

  std::map<std::string,struct Frame> tile_frames_catalog;



  std::map<int, struct Frame> read_texture_spritesheet(const char *img_json_path, std::string type)
  {
    std::ifstream json_file(img_json_path);
    std::ostringstream tmp;
    tmp << json_file.rdbuf();
    std::string s = tmp.str();

    std::regex e("\\{(.*) (.*) (.*) (.*) (.*) (.*) (.*) (.*)\\}");
    std::regex e2(": .([0-9])*");
    std::regex_token_iterator<std::string::iterator> rend;
    std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), e);
    std::map<int, struct TileFrame> frames;

    while (a != rend)
    {

      struct TileFrame f;
      std::string s(*a++);
      std::regex_token_iterator<std::string::iterator> numbers(s.begin(), s.end(), e2);
      std::vector<int> numbers_v = {};
      while (numbers != rend)
      {
        std::string number_string(*numbers++);
        std::string number_clean;
        number_clean = number_string.replace(0, 2, "");
        numbers_v.push_back(std::stoi(number_clean));
      }

      f.x = numbers_v[0];
      f.y = numbers_v[1];
      f.w = numbers_v[2];
      f.h = numbers_v[3];
      f.type = numbers_v[4];
      f.solid = numbers_v[5];
      f.x_end = numbers_v[6];
      f.texture_id = (float)numbers_v[7];

      f.norm_x_start =  (float)f.x/196.0;
      f.norm_x_end =  (float)f.x_end/196.0;
      

      frames.insert({f.type, f});
    }
    return frames;
  }







}












#endif