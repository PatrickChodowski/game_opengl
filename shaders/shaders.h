#ifndef SHADERS_H
#define SHADERS_H

namespace shaders   
{
  GLuint shading_program;
  std::string read_file_as_string(const char *filename)
  {
    #ifdef TARGET_OS_MAC
      std::string version = "330";
      std::string slots = "16";
    #endif

    #ifdef __linux__
      std::string version = "460";
      std::string slots = "32";
    #endif
      
    std::string shader_source = utils::read_text_file(filename);

    shader_source = std::regex_replace(shader_source, std::regex("\\$version"), version);
    shader_source = std::regex_replace(shader_source, std::regex("\\$slots"), slots);

    return shader_source;
  }

  GLuint program_check(GLuint program)
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
  }

  GLuint compile_shader(GLenum type, GLsizei nsources, const char **sources)
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

  // loads a shader from file and returns the compiled shader
  GLuint get_shader(GLenum eShaderType, const char *filename)
  {
    std::string shader_string = read_file_as_string(filename);
    const char *shaderSource = shader_string.c_str();
    GLuint shader = compile_shader(eShaderType, 1, &shaderSource);
    return shader;
  }

  //Get and build custom program from 2 files
  GLuint custom_shaders(const char *shader_name)
  {
    GLuint vertexShader;
    GLuint fragmentShader;

    std::string vsPath = std::string("./shaders/")+std::string(shader_name)+std::string(".vert");
    std::string fsPath = std::string("./shaders/")+std::string(shader_name)+std::string(".frag");

    vertexShader = get_shader(GL_VERTEX_SHADER, vsPath.c_str());
    fragmentShader = get_shader(GL_FRAGMENT_SHADER, fsPath.c_str());
    shading_program = glCreateProgram();

    glAttachShader(shading_program, vertexShader);
    glAttachShader(shading_program, fragmentShader);
    glLinkProgram(shading_program);
 
    //Error Checking
    GLuint status;
    status = program_check(shading_program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (status == GL_FALSE){
      logger::print("CustomShaderException: Shader status is FALSE", 2);
      return 0;
    };
    return shading_program;
  }

  struct ShaderData
  {
    int id;
    std::string name;
    GLuint gl_shader_id;

    JS_OBJ(id, name, gl_shader_id);
  };
  std::map<int, ShaderData> Catalog = {};

  void read_shaders_data(std::string name)
  {
    ShaderData SD;
    std::string data_path = "./shaders/data/"+name+".json";
    std::string json_data = utils::read_text_file(data_path);
    JS::ParseContext context(json_data);
    context.parseTo(SD);

    // add to catalog
    Catalog.insert({SD.id, SD});

    if(LOGGING == 0)
    {
      std::cout << "Read-in shader id: " << SD.id << ", name: " << SD.name << ", gl_shader_id: " << SD.gl_shader_id << std::endl;
    }
  }


  void init()
  {
    logger::print("READING SHADERS");
    std::vector<std::string> shaders_list = utils::list_files("shaders/data/");
    for(int s=0; s < shaders_list.size(); s++)
    {
      read_shaders_data(shaders_list[s]);
    };

    // sorted by ID:
    for (auto const& x : shaders::Catalog)
    {
      shaders::Catalog[x.first].gl_shader_id = shaders::custom_shaders(x.second.name.c_str());
    }

    // for(int s=0; s < shaders_list.size(); s++)
    // {
    //   shaders::Catalog[s].gl_shader_id = shaders::custom_shaders(shaders_list[s].c_str());
    // }
    glReleaseShaderCompiler();
  }



  void drop()
  {
    glDeleteProgram(shaders::Catalog[CURRENT_SHADER_ID].gl_shader_id);
  }

}

#endif
