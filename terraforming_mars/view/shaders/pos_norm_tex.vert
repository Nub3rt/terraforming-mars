#version 430

layout( location = 0 ) in vec3 vs_in_pos;
layout( location = 1 ) in vec3 vs_in_norm;
layout( location = 2 ) in vec2 vs_in_tex;

out vec3 vs_orig_pos;
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

uniform mat4 world;
uniform mat4 world_it;
uniform mat4 view_proj;

uniform mat4 rotation;
uniform bool ocean;
uniform float special_z;

void main() {
    vec3 pos = vs_in_pos;
    if ( ocean && pos.z < special_z ) {
        pos.z = special_z;
        vs_out_norm = (world_it * vec4( 0.0, 0.0, 1.0, 0.0 )).xyz;
    } else
        vs_out_norm = (world_it * vec4( vs_in_norm, 0.0 )).xyz;

    gl_Position = view_proj * world * vec4( pos, 1.0 );

    vs_orig_pos = vs_in_pos;
    vs_out_pos  = (world * vec4( pos, 1.0 )).xyz;


    vec2 uv_to_xy = vec2( sqrt( 3.0 ) * (vs_in_tex.x - 0.5), 2 * vs_in_tex.y - 1 );
    vec4 corrected = rotation * vec4( uv_to_xy, 0.0, 1.0 );
    vs_out_tex = vec2( corrected.x / sqrt( 3.0 ) + 0.5, (corrected.y + 1.0) / 2.0 );
}
