#ifndef TEXTURES_H
#define TEXTURES_H


namespace textures
{
  struct Frame
  {
    int frame_id;
    int x;
    int y;
    int w;
    int h;
    int is_solid;

    JS_OBJ(frame_id, x, y, w, h, is_solid);
  };


  struct TextureData
  {
    int id;
    std::string type;
    std::string name;
    int width;
    int height;
    std::vector<Frame> frames;

    JS_OBJ(id, type, name, width, height, frames);
  };

  std::string read_text_file(std::string path)
  {
    std::ifstream json_file(path);
    std::ostringstream tmp;
    tmp << json_file.rdbuf();
    std::string s = tmp.str();
    return s;
  }

  void read_texture(std::string name)
  {
    TextureData TD;
    std::string data_path = "./assets/data/"+name+".json";
    std::string json_data = read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(TD);

    if(LOGGING == 0)
    {
      std::cout << "Read-in texture id: " << TD.id << ", type: " << TD.type << ", name: " <<
       TD.name << ", width: " << TD.width << ", height: " << TD.height << ",  frames count: " << TD.frames.size() << std::endl;

      for(int f=0; f<TD.frames.size(); f++)
      {
       std::cout <<  "frame_id: " << TD.frames[f].frame_id << ", x: " 
       <<  TD.frames[f].x << ", y: "<< TD.frames[f].y << ", w: " 
       << TD.frames[f].w << ", h: "<< TD.frames[f].h << ", is_solid: " << TD.frames[f].is_solid << std::endl;
      }

    }

  }







  // std::map<int, struct Frame> read_texture_spritesheet(const char *img_json_path, std::string type)
  // {
  //   std::ifstream json_file(img_json_path);
  //   std::ostringstream tmp;
  //   tmp << json_file.rdbuf();
  //   std::string s = tmp.str();

  //   std::regex e("\\{(.*) (.*) (.*) (.*) (.*) (.*) (.*) (.*)\\}");
  //   std::regex e2(": .([0-9])*");
  //   std::regex_token_iterator<std::string::iterator> rend;
  //   std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), e);
  //   std::map<int, struct TileFrame> frames;

  //   while (a != rend)
  //   {

  //     struct TileFrame f;
  //     std::string s(*a++);
  //     std::regex_token_iterator<std::string::iterator> numbers(s.begin(), s.end(), e2);
  //     std::vector<int> numbers_v = {};
  //     while (numbers != rend)
  //     {
  //       std::string number_string(*numbers++);
  //       std::string number_clean;
  //       number_clean = number_string.replace(0, 2, "");
  //       numbers_v.push_back(std::stoi(number_clean));
  //     }

  //     f.x = numbers_v[0];
  //     f.y = numbers_v[1];
  //     f.w = numbers_v[2];
  //     f.h = numbers_v[3];
  //     f.type = numbers_v[4];
  //     f.solid = numbers_v[5];
  //     f.x_end = numbers_v[6];
  //     f.texture_id = (float)numbers_v[7];

  //     f.norm_x_start =  (float)f.x/196.0;
  //     f.norm_x_end =  (float)f.x_end/196.0;
      

  //     frames.insert({f.type, f});
  //   }
  //   return frames;
  // }







}












#endif