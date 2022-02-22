#version 330

// Input
in vec3 vs_color;

// Output
layout(location = 0) out vec4 fs_color;

void main()
{
    fs_color = vec4(vs_color, 1.0f);
}
