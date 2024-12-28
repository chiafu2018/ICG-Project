#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoord;
    vec4 outColor; 
} gs_in[];

out vec2 TexCoord; 
out vec4 outColor;

// this is just a simple geomtry shader

void main() {    

    gl_Position = gl_in[0].gl_Position;
    TexCoord = gs_in[0].TexCoord;
    outColor = gs_in[0].outColor;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    TexCoord = gs_in[1].TexCoord;
    outColor = gs_in[1].outColor;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    TexCoord = gs_in[2].TexCoord;
    outColor = gs_in[2].outColor;

    EmitVertex();
    EndPrimitive();
}  