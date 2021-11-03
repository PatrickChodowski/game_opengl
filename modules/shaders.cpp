#include "shaders.h"
#include <map>
#include <vector>
#include <string>
#include <regex> 
#include <SDL2/SDL.h>

#include "utils.h"
#include "../dependencies/json_struct.h"

#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


namespace shaders2
{
  std::map<int, ShaderData> shaders = {};
  GLuint shading_program;
  int CURRENT_SHADER_ID = 0;

  void read_data(std::string name)
  {
    ShaderData SD;
    std::string data_path = "./shaders/data/"+name+".json";
    std::string json_data = utils2::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);
    shaders2::shaders.insert({SD.id, SD});
  }

  void init()
  {
    std::vector<std::string> shaders_list = utils2::list_json_files("shaders/data/");
    for(int s=0; s < shaders_list.size(); s++)
    {
      shaders2::read_data(shaders_list[s]);
    };

    for (auto const& x : shaders2::shaders)
    {
      shaders2::shaders[x.first].gl_shader_id = shaders2::build(x.second.name.c_str());
    }
    glReleaseShaderCompiler();
  };

  void drop()
  {
    glDeleteProgram(shaders2::shaders[CURRENT_SHADER_ID].gl_shader_id);
  }

  std::string _read_file_as_string(const char *filename)
  {
    #ifdef TARGET_OS_MAC
      std::string version = "330";
      std::string slots = "16";
    #endif

    #ifdef __linux__
      std::string version = "460";
      std::string slots = "32";
    #endif
      
    std::string shader_source = utils2::read_text_file(filename);

    shader_source = std::regex_replace(shader_source, std::regex("\\$version"), version);
    shader_source = std::regex_replace(shader_source, std::regex("\\$slots"), slots);

    return shader_source;
  };

  GLuint _shader_program_check(GLuint program)
  {
    //Error Checking
    GLint status;
    glValidateProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
      GLint len;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
      if (len > 1)
      {
        char *log;
        log = (char*)malloc(len);
        glGetProgramInfoLog(program, len, &len, log);
        fprintf(stderr, "%s\n\n", log);
        free(log);
      }
      SDL_Log("Error linking shader default program.\n");
      return GL_FALSE;
    }
    return GL_TRUE;
  };

  GLuint _compile_shader(GLenum type, GLsizei nsources, const char **sources)
  {

    GLuint shader;
    GLint success, len;
    GLsizei i, srclens[nsources];

    for (i = 0; i < nsources; ++i)
    {
      srclens[i] = (GLsizei)strlen(sources[i]);
    }
    shader = glCreateShader(type);
    glShaderSource(shader, nsources, sources, srclens);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
      if (len > 1)
      {
        char *log;
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, NULL, log);
        fprintf(stderr, "%s\n\n", log);
        free(log);
      }
      SDL_Log("Error compiling shader.\n");
    }
    SDL_Log("shader: %u", shader);
    return shader;
  }

  GLuint _get_shader(GLenum shader_type, const char *filename)
  {
    std::string shader_string = _read_file_as_string(filename);
    const char *shaderSource = shader_string.c_str();
    GLuint shader = _compile_shader(shader_type, 1, &shaderSource);
    return shader;
  }

  GLuint build(const char *shader_name)
  {
    GLuint vertexShader;
    GLuint fragmentShader;

    std::string vsPath = std::string("./shaders/")+std::string(shader_name)+std::string(".vert");
    std::string fsPath = std::string("./shaders/")+std::string(shader_name)+std::string(".frag");

    vertexShader = shaders2::_get_shader(GL_VERTEX_SHADER, vsPath.c_str());
    fragmentShader = shaders2::_get_shader(GL_FRAGMENT_SHADER, fsPath.c_str());
    shading_program = glCreateProgram();

    glAttachShader(shading_program, vertexShader);
    glAttachShader(shading_program, fragmentShader);
    glLinkProgram(shading_program);
 
    //Error Checking
    GLuint status;
    status = _shader_program_check(shading_program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (status == GL_FALSE){
      //logger::print("CustomShaderException: Shader status is FALSE", 2);
      return 0;
    };
    return shading_program;
  };

}
