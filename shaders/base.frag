
in vec4 out_color;
in vec2 out_tex_coord;
in float out_texture_id;
out vec4 frag_color;


// one texture only as uniform
uniform sampler2D textures[2];

void main()
{
  int texture_index = int(out_texture_id);
  // frag_color = out_color;
  // frag_color = texture(texture0, out_tex_coord);
  frag_color = texture(textures[texture_index], out_tex_coord);
  // frag_color = vec4(out_tex_coord, 0.5f, 1.0f);
}

