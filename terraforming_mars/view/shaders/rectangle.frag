#version 430

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D image;

void main() {
    fs_out_col = texture( image, vs_out_tex );

    if ( fs_out_col.a < 0.1 )
        discard;
}
