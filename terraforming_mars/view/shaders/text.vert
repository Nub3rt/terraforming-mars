#version 430

layout ( location = 0 ) in vec4 vs_in_pos_tex; // <vec2 pos, vec2 tex>

out vec2 vs_out_tex;

void main() {
    gl_Position = vec4( vs_in_pos_tex.xy, 1.0, 1.0 );

    vs_out_tex = vs_in_pos_tex.zw;
}
