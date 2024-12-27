#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
uniform float tint;

const float size = 0.2;

uniform mat4 projection;

out vec2 fragmentTexCoord;

void main()
{
    // gl_In = struct { gl_Position, gl_PointSize, gl_ClipDistance };

    float dynamicSize = mix(size, 0.6, 1-tint);
        // Exponential scaling for finer size reduction
    //float dynamicSize = size * pow(0.5, tint);
    


    //bottom left
    gl_Position = projection * (vec4(-dynamicSize, -dynamicSize, 0.0, 0.0) + gl_in[0].gl_Position);
    fragmentTexCoord = vec2(0.0, 0.0);
    EmitVertex();

    //bottom right
    gl_Position = projection * (vec4(dynamicSize, -dynamicSize, 0.0, 0.0) + gl_in[0].gl_Position);
    fragmentTexCoord = vec2(1.0, 0.0);
    EmitVertex();

    //top left
    gl_Position = projection * (vec4(-dynamicSize, dynamicSize, 0.0, 0.0) + gl_in[0].gl_Position);
    fragmentTexCoord = vec2(0.0, 1.0);
    EmitVertex();

    //top right
    gl_Position = projection * (vec4(dynamicSize, dynamicSize, 0.0, 0.0) + gl_in[0].gl_Position);
    fragmentTexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}