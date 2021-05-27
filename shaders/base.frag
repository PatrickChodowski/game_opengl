
in vec4 out_color;
in vec2 out_tex_coord;
in float out_texture_id;
out vec4 frag_color;


// one texture only as uniform
uniform sampler2D textures[1];

uniform sampler2D texture0;
uniform float red_color;

void main()
{
  int texture_index = int(out_texture_id);
  // frag_color = out_color;
  frag_color = texture(texture0, out_tex_coord);
  // frag_color.a = 0.5;
  // frag_color = vec4(out_texture_id,out_texture_id,out_texture_id,1.0f);
  //   frag_color = vec4(red_color, 0.5f, 0.5f, 1.0f);
  // frag_color = vec4(out_tex_coord, 0.5f, 1.0f);
}

