#version 430

in vec3 vs_orig_pos;
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in float vs_out_on_edge;

out vec4 fs_out_col;

uniform vec4 tex_ranges;
uniform sampler2D terrain_mars;
uniform vec4 mars_ranges;
uniform sampler2D terrain_special;
uniform vec3 border_color;

ivec2 axial_round( vec2 qr ) {
    float q = qr.x;
    float r = qr.y;
    float s = - q - r;

    float q_round = round( q );
    float r_round = round( r );
    float s_round = round( s );

    float q_diff = abs( q_round - q );
    float r_diff = abs( r_round - r );
    float s_diff = abs( s_round - s );

    if ( q_diff > r_diff && q_diff > s_diff )
        q_round = - r_round - s_round;
    else if ( r_diff > s_diff )
        r_round = - q_round - s_round;
    else
        s_round = - q_round - r_round;

    return ivec2( q_round, r_round );
}

void main() {
    vec2 uv = vec2(
        mix( tex_ranges.x, tex_ranges.y, 1.0 - vs_out_tex.x ),
        mix( tex_ranges.z, tex_ranges.w, 1.0 - vs_out_tex.y )
    );
    uv = vec2(
        mix( mars_ranges.x, mars_ranges.y, uv.x ),
        mix( mars_ranges.z, mars_ranges.w, uv.y )
    );
    vec4 tex = texture( terrain_mars, uv );


    vec2 xy = vs_orig_pos.xz;
    mat2 pixel_to_hex = mat2( sqrt( 3.0 ) / 3.0, 0.0, -1.0 / 3.0, 2.0 / 3.0 );
    vec2 qr = pixel_to_hex * xy / 0.95;
    ivec2 iqr = axial_round( qr );

    if ( iqr.x != 0 || iqr.y != 0 ) {
        tex = vec4( border_color, 1.0 );
    }

    fs_out_col = tex;
}
