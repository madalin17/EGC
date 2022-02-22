#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int spotlight;
uniform float cut_off;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, world_position);
    vec3 H = normalize(L + V);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);
    float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = 2 * material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float intensity = 0;
    if (spotlight == 1) {
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);

        if (spot_light > spot_light_limit) {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            intensity = pow(linear_att, 2);
        }
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    else if (spotlight == 0) {
        float d = distance(light_position, world_position);
        float k = 1 + 1 / pow(d, 2);
        intensity = ambient_light + k * (diffuse_light + specular_light);
    }

    // TODO(student): Write pixel out color
    out_color = vec4(object_color * intensity, 1);

}
