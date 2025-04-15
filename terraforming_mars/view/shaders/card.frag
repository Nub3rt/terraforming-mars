#version 430

in vec3 vs_out_pos;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D image;
uniform float elapsed;

uniform float stride_x;
uniform float stride_y;
uniform int index_x;
uniform int index_y;

uniform bool faded;
uniform bool highlight;
uniform vec3 highlight_color;

void main() {
    vec2 pos = vec2(
        (vs_out_tex.x + index_x) * stride_x,
        (vs_out_tex.y + index_y) * stride_y
    );
    fs_out_col = texture( image, pos );

    if ( fs_out_col.a < 0.5 ) {
        if ( !highlight )
            discard;

        float time = mod( elapsed, 3.0 );
        if ( time > 1.5 )
            time = 3.0 - time;

        time /= 1.5;
        time = time < 0.5 ? 2 * time * time : 1 - (-2 * time + 2) * (-2 * time + 2) / 2;
        time *= 1.5;

        float threshold = 0.9 + (time - 1.5) * 0.05;
        float multiplier = 1.0 / (1.0 - threshold);

        float x = abs( vs_out_pos.x );
        float y = abs( vs_out_pos.y );

        float intensity = 1.0;
        if ( x >= threshold && y >= threshold )
            intensity = (1.0 - y) * multiplier * (1.0 - x) * multiplier;
        else if ( x >= threshold )
            intensity = (1.0 - x) * multiplier;
        else if ( y >= threshold )
            intensity = (1.0 - y) * multiplier;

        fs_out_col = vec4( highlight_color, intensity );
    }

    if ( faded ) {
        fs_out_col *= 0.5;
        fs_out_col.a = 1.0;
    }
}
