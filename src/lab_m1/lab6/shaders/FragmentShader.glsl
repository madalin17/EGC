#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 norm_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(frag_color, 1.0f);
    //out_color = vec4(norm_color, 1.0f);

}