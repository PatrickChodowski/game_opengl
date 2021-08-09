#version $version core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in float in_tile_type;
layout (location = 3) in vec2 in_tex_coord;
layout (location = 4) in float in_texture_id;
layout (location = 5) in float in_is_clicked;
layout (location = 6) in float in_type_id;
layout (location = 7) in float in_is_static;

out vec4 out_color;
out float out_tile_type;
out vec2 out_tex_coord;
out float out_texture_id;
out float out_is_clicked;
out float out_type_id;
out float out_is_static;


uniform mat4 static_mvp;
uniform mat4 dynamic_mvp;
uniform mat4 zoom_mvp;

void main(void) {
    if(in_is_static == 1.0){
      gl_Position = static_mvp * vec4(in_position.x, in_position.y, in_position.z, 1.0);
    } else if (in_is_static == 0.0){
      gl_Position = dynamic_mvp * vec4(in_position.x, in_position.y, in_position.z, 1.0);
    } else {
      gl_Position = zoom_mvp * vec4(in_position.x, in_position.y, in_position.z, 1.0);
    }
    out_color = vec4(in_color);
    out_tile_type = in_tile_type;
    out_tex_coord = vec2(in_tex_coord);
    out_texture_id = in_texture_id;
    out_is_clicked = in_is_clicked;
    out_type_id = in_type_id;
    out_is_static = in_is_static;
}