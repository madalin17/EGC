#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform int exists_1;
uniform int exists_2;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color_1 = vec4(0);
    if (exists_1 == 1) {
        color_1 = texture2D(texture_1, texcoord);
        if (color_1.a < 0.5f) {
            discard;
        }
    }
    
    vec4 color_2 = vec4(0);
    if (exists_2 == 1) {
        color_2 = texture2D(texture_2, texcoord);
        if (color_2.a < 0.5f) {
            discard;
        }
    }

    if (exists_1 == 1 && exists_2 == 1) {
        out_color = mix(color_1, color_2, 0.5f);
    } else if (exists_1 == 1) {
        out_color = color_1;
    } else if (exists_2 == 1) {
        out_color = color_2;
    }
}
