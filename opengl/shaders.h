#ifndef SHADERS_H
#define SHADERS_H

namespace shaders   
{
  GLuint shading_program;

  const char *read_file(const char *filename)
  {
    long length = 0;
    char *result = NULL;
    FILE *file = fopen(filename, "r");
    if (file)
    {
      int status = fseek(file, 0, SEEK_END);
      if (status != 0)
      {
        fclose(file);
        return NULL;
      }
      length = ftell(file);
      status = fseek(file, 0, SEEK_SET);
      if (status != 0)
      {
        fclose(file);
        return NULL;
      }
      result = (char*)malloc((length + 1) * sizeof(char));
      if (result)
      {
        size_t actual_length = fread(result, sizeof(char), length, file);
        result[actual_length++] = '\0';
      }
      fclose(file);
      return result;
    }
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't read %s", filename);
    return NULL;
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
    const char *shaderSource = read_file(filename);
    std::cout << eShaderType << std::endl;
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

    logger::print("Shading program before return: " + std::to_string(shading_program), 1);
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
    for(int s=0; s < shaders_list.size(); s++)
    {
      shaders::Catalog[s].gl_shader_id = shaders::custom_shaders(shaders_list[s].c_str());
    }
    glReleaseShaderCompiler();
  }



  void drop()
  {
    glDeleteProgram(shaders::Catalog[0].gl_shader_id);
  }

}

#endif
