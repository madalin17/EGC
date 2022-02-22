#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms to describe the floor cell
uniform vec3 object_color;
uniform float material_ke;

// Uniforms for spotlight properties
uniform vec3 spotlight_positions[4];
uniform vec3 spotlight_directions[4];
uniform vec3 spotlight_colors[4];
uniform float cut_off;

// Uniforms for globe properties
uniform sampler2D texture;
uniform vec3 globe_position;
uniform float time;

// Output
layout(location = 0) out vec4 out_color;

vec4 PointLightContribution()
{
    // Dance-floor cell light contribution
    vec3 final_color = material_ke * object_color;

    // Spotlights light contribution
    for (int i = 0; i < 4; i++) {
        vec3 L = normalize(spotlight_positions[i] - world_position);
        float spot_light = dot(-L, spotlight_directions[i]);
        float spot_light_limit = cos(cut_off);

        if (spot_light > spot_light_limit) {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float intensity = pow(linear_att, 2);
            final_color += intensity * spotlight_colors[i];
        }
    }

    // Globe light contribution
    vec3 globe_light_dir = world_position - globe_position;
    vec2 texcoord;
    texcoord.x = time + (1.0 / (2 * 3.14159)) * atan(globe_light_dir.x, globe_light_dir.z);
    texcoord.y = (1.0 / 3.14159) * acos(globe_light_dir.y / length (globe_light_dir));
    final_color += texture2D(texture, texcoord).xyz;

	return vec4(final_color, 1);
}

void main()
{
    out_color = PointLightContribution();
}
