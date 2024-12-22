#version 330 core

// TODO 2
// Implement Bling-Phong shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;  
out vec3 world_normal;
out vec3 world_position; 

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;

	// transform the normal from object coordinate to world coordinate
    world_normal = normalize(mat3(transpose(inverse(model))) * aNormal);

	// Calculate fragment position in world space
    world_position = vec3(model * vec4(aPos, 1.0));
}