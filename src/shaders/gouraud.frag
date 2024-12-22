#version 330 core

// TODO 3:
// Implement Gouraud shading

out vec4 FragColor;

in vec2 TexCoord; 
in vec3 I; 

in vec4 testPosition;

uniform sampler2D ourTexture;

void main()
{
    if(testPosition[2]<0.8){
        FragColor = texture(ourTexture, TexCoord) * vec4(255, 1.0, 1.0, 1.0); 
    }else{
        FragColor = texture(ourTexture, TexCoord) * vec4(100, 250, 30, 1.0); 
    }
    
    
    
    // FragColor = texture(ourTexture, TexCoord) * vec4(I, 1.0);

}