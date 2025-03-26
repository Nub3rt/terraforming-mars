#version 430

layout( location = 0 ) in vec3 vs_in_pos;
layout( location = 1 ) in vec3 vs_in_norm;
layout( location = 2 ) in vec2 vs_in_tex;
layout( location = 3 ) in float vs_in_on_edge;

out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out float vs_out_on_edge;

uniform mat4 world;
uniform mat4 world_it;
uniform mat4 view_proj;

void main() {
    gl_Position = view_proj * world * vec4( vs_in_pos, 1.0 ); 

    vs_out_pos  = (world    * vec4( vs_in_pos,  1.0 )).xyz;
    vs_out_norm = (world_it * vec4( vs_in_norm, 0.0 )).xyz;
    vs_out_tex = vs_in_tex;
    vs_out_on_edge = vs_in_on_edge;
}
