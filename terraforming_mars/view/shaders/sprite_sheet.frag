#version 430

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D image;
uniform float stride_x;
uniform float stride_y;
uniform int index_x;
uniform int index_y;

void main() {
    vec2 pos = vec2(
        (vs_out_tex.x + index_x) * stride_x,
        (vs_out_tex.y + index_y) * stride_y
    );
    fs_out_col = texture( image, pos );

    if ( fs_out_col.a < 0.5 )
        discard;
}
