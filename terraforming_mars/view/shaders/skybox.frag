#version 430

in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform samplerCube skybox_texture;

void main()
{
    vec3 direction = normalize( vs_out_pos );

    fs_out_col = texture( skybox_texture, direction );
}
