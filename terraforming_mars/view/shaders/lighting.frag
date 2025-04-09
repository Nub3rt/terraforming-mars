#version 430

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in float vs_out_on_edge;

out vec4 fs_out_col;

uniform sampler2D image;
uniform vec3 color;

void main() {
//    vec4 tex = texture( image, vs_out_tex );
    vec4 tex = vec4( color, 1.0 );

    if ( vs_out_on_edge > 0.97 ) {
        tex = vec4( 1.0, 1.0, 1.0, 1.0 );
    }

    fs_out_col = tex;
}
