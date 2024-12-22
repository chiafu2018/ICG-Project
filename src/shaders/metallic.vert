#version 330 core

// TODO 5: 
// Implement Metallic shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 world_normal;
out vec3 world_position;
out vec2 TexCoord;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord; 

    world_normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    world_position = vec3(model * vec4(aPos, 1.0));
}
