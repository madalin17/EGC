#version 330

// Uniform properties
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Make spotlight transparent
    out_color = vec4(object_color, 0.3f);
}
