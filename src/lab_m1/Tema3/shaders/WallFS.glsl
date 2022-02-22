#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_positions[3];
uniform vec3 light_colors[3];
uniform float light_radius;

// Uniforms for spotlight properties
uniform vec3 spotlight_positions[4];
uniform vec3 spotlight_directions[4];
uniform vec3 spotlight_colors[4];
uniform float cut_off;

// Uniforms for globe properties
uniform sampler2D texture;
uniform vec3 globe_position;
uniform float time;

// Uniforms for material properties and camera position
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Output
layout(location = 0) out vec4 out_color;

vec4 PointLightContribution()
{
    vec3 final_color = vec3(0);
    // Dance-floor cells light contribution
    for (int i = 0; i < 3; i++) {
        vec3 L = normalize(light_positions[i] - world_position);
        vec3 V = normalize(eye_position - world_position);
        vec3 H = normalize(L + V);

        float diffuse_light = material_kd * max(dot(world_normal, L), 0);
        float specular_light = 0;
        if (diffuse_light > 0) {
            specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
        }

        float att = 0.0f;
        float dist = distance(world_position, light_positions[i]);
        if (dist < light_radius) {
	        att = pow(light_radius - dist, 0.3f);
        }

        float intensity = att * (diffuse_light + specular_light);
        final_color += intensity * light_colors[i];
    }

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
