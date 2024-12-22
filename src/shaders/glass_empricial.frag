#version 330 core

// TODO 6-2
// Implement Glass-Empricial shading


out vec4 FragColor;

in vec2 TexCoord;
in vec3 world_normal;
in vec3 world_position;

uniform vec3 light_position; 
uniform vec3 camera_position;
uniform samplerCube skybox;
uniform sampler2D ourTexture;

void main() 
{
    vec3 I = normalize(world_position - camera_position);
    vec3 R = reflect(I, world_normal);
    vec3 light_dir = normalize(light_position - world_position); 

    float n = 1 / 1.52;
    float K = 1 - n * n * (1 - (dot(I, world_normal) * dot(I, world_normal)));

    vec3 T;
    if (K < 0) {
        T = vec3(0.0); 
    } else {
        T = n * I - (n * dot(I, world_normal) + sqrt(K)) * world_normal;
    }

    vec3 C_reflect = texture(skybox, R).rgb;
    vec3 C_refract = texture(skybox, T).rgb;

    float R_sita = max(0, min(1, 0.2 + 0.7 * pow(1+dot(I, world_normal), 2)));
    FragColor = vec4(R_sita * C_reflect + (1-R_sita) * C_refract, 1.0);

}	