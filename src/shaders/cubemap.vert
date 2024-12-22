#version 330 core

// TODO 4-1
// Implement CubeMap shading

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 skyboxview;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * skyboxview * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}