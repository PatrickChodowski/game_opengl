#version 460 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in float in_tile_type;
layout (location = 3) in vec2 in_tex_coord;
layout (location = 4) in float in_texture_id;

out vec4 out_color;
out float out_tile_type;
out vec2 out_tex_coord;
out float out_texture_id;


uniform mat4 mvp;

void main(void) {
    gl_Position = mvp * vec4(in_position.x, in_position.y, in_position.z, 1.0);
    out_color = vec4(in_color);
    out_tile_type = in_tile_type;
    out_tex_coord = vec2(in_tex_coord);
    out_texture_id = in_texture_id;
}
