#version $version core

in vec4 out_color;
in vec2 out_tex_coord;
in float out_texture_id;
in float out_is_clicked;
in float out_type_id;

out vec4 frag_color;


// one texture only as uniform
uniform sampler2D textures[$slots];

void main()
{
  // if type is level, use texture
  int texture_index = int(out_texture_id);
  if(out_type_id == 0.0)
  {
    frag_color = texture(textures[texture_index], out_tex_coord);
  }
  // when type is text:
  else if (out_type_id == 2.0)
  {
    frag_color = vec4(out_color.rgb, texture(textures[texture_index], out_tex_coord).r);
  }


  //frag_color = texture(textures[texture_index], out_tex_coord);
  //frag_color.r = (1-out_is_clicked)*frag_color.r; // for clicking 
  //frag_color = texture(textures[texture_index], out_tex_coord);
  // frag_color = texture(textures[texture_index], out_tex_coord);
  // frag_color.g = 0.2f;
  // 1,1,1,1 for white
  // 0,0,0,1 for black
  // frag_color = vec4((texture_index/4.0f), (texture_index/4.0f), (texture_index/4.0f), 1.0f);
}