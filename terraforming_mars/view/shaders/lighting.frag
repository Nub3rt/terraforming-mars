#version 430

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D color;

void main() {
    fs_out_col = texture( color, vs_out_tex );
}
