#version 430

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D text;
uniform vec4 text_col;

void main() {
    vec4 sampled = vec4( 1.0, 1.0, 1.0, texture( text, vs_out_tex ).r );
    fs_out_col = vec4( text_col ) * sampled;
}
