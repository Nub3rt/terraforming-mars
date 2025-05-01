#version 430

in vec3 vs_orig_pos;
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in float vs_out_on_edge;

out vec4 fs_out_col;

uniform vec3 camera_pos;
uniform vec4 light_pos;

uniform vec3 la;
uniform vec3 ld;
uniform vec3 ls;

uniform vec3 ka = vec3( 1.0 );
uniform vec3 kd = vec3( 1.0 );
uniform vec3 ks = vec3( 1.0 );

uniform float shininess;


uniform bool special;
uniform bool ocean;
uniform float elapsed;
uniform float mars_to_special;
uniform float special_z;
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
    vec3 la;
    vec3 ld;
    vec3 ls;
    vec3 ks;
    float shininess;

    vec2 uv = vec2(
        mix( tex_ranges.x, tex_ranges.y, vs_out_tex.x ),
        mix( tex_ranges.z, tex_ranges.w, vs_out_tex.y )
    );
    uv = vec2(
        mix( mars_ranges.x, mars_ranges.y, uv.x ),
        mix( mars_ranges.z, mars_ranges.w, uv.y )
    );
    vec4 tex;
    if ( ocean && vs_orig_pos.z < special_z ) {
        uv += elapsed * 0.001;
        tex = texture( terrain_special, uv );
        la = vec3( 0.5 );
        ld = vec3( 1.0 );
        ls = vec3( 1.0 );
        ks = vec3( 1.0 );
        shininess = 35.0;
    } else {
        if ( mars_to_special == 0.0 )
            tex = texture( terrain_mars, uv );
        else if ( mars_to_special == 1.0 )
            tex = texture( terrain_special, uv );
        else
            tex = mix(
                texture( terrain_mars, uv ),
                texture( terrain_special, uv ),
                mars_to_special
            );
        la = vec3( 1.2 );
        ld = vec3( 1.0 );
        ls = vec3( 0.8 );
        ks = vec3( 0.2 );
        shininess = 4.0;
     }


    if ( !special ) {
        vec2 xy = vs_orig_pos.xy;
        mat2 pixel_to_hex = mat2( sqrt( 3.0 ) / 3.0, 0.0, -1.0 / 3.0, 2.0 / 3.0 );
        vec2 qr = pixel_to_hex * xy / 0.95;
        ivec2 iqr = axial_round( qr );

        if ( iqr.x != 0 || iqr.y != 0 ) {
            fs_out_col = vec4( border_color, 1.0 );
            return;
        }
    }


    vec3 ka = vec3( tex );
    vec3 kd = vec3( tex );


    vec3 normal = normalize( vs_out_norm );

    vec3 ambient = la * ka;

    vec3 to_light = light_pos.xyz;

    float diffuse_factor = max( dot( to_light, normal ), 0.0 );
    vec3 diffuse = diffuse_factor * ld * kd;

    vec3 view_dir = normalize( camera_pos - vs_out_pos );
    vec3 reflect_dir = reflect( -to_light, normal );

    float specular_factor = pow( max( dot( view_dir, reflect_dir ), 0.0), shininess);
    vec3 specular = specular_factor * ls * ks;

    fs_out_col = vec4( ambient + diffuse + specular, 1.0 ) * tex;
}
