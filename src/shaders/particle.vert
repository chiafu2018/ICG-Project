#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = view * model * vec4(vec3(0.0), 1.0);
}
