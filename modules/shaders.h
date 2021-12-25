
#include <string>
#include "../dependencies/json_struct.h"
#include "../dependencies/parallel_hashmap/phmap.h"

#include <GL/glew.h> 
#ifdef TARGET_OS_MAC
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#ifndef MODULES_SHADERS_H
#define MODULES_SHADERS_H

namespace shaders
{
  // Contains information about the shader
  struct ShaderData
  {
    int id;
    GLuint gl_shader_id;
    std::string name;

    JS_OBJ(id, gl_shader_id, name);
  };

  extern phmap::flat_hash_map<int, ShaderData> shaders;
  extern GLuint shading_program;

  // Reads shaders data
  void read_data(std::string name);

  // Gets list of shaders and reads data about them. Fills out shaders data.
  void init();

  // Delete shaders
  void drop();

  // Reads shaders file as string
  std::string _read_file_as_string(const char *filename);

  // Performs shaders check
  GLuint _shader_program_check(GLuint program);

  // Compiles shader from read file
  GLuint _compile_shader(GLenum type, GLsizei nsources, const char **sources);

  // Loads a shader from file and returns the compiled shader
  GLuint _get_shader(GLenum shader_type, const char *filename);

  // Build shader from custom files
  GLuint build(const char *shader_name);

}


#endif