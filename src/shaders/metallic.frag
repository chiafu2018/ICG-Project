#version 330 core

// TODO 5:
// Implement Metallic shading

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

    vec3 C_model = texture(ourTexture, TexCoord).rgb;
    vec3 C_reflect = texture(skybox, R).rgb;

    vec3 light_dir = normalize(light_position - world_position); 

    float B = 0.2;
    if(dot(light_dir, world_normal) > 0){
        B += dot(light_dir, world_normal);
    }

    FragColor = vec4((0.4*B*C_model+0.6*C_reflect), 1.0);
}	

