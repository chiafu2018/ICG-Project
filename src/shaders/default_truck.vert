#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aVertexColor;

out vec2 TexCoord;
out vec4 outColor; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float squeezeFactor;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   TexCoord = aTexCoord;
   
   // each vertex has its own color, defined in obj file. 
   // if that the object is gonna to bind with the texture, we are going to set outColor to vec4(-1.0, -1.0, -1.0, -1.0)
   float vertexColor = aVertexColor; 

   if(vertexColor == 0.0){
      outColor = vec4(1.0, 0.67, 0.1, 1.0); 
   }else if(vertexColor == 1.0){
      outColor = vec4(0, 0, 0, 1.0);
   }else if(vertexColor == 2.0){
      outColor = vec4(0.8, 0.9, 1.0, 0.3);
   }else if(vertexColor == 3.0){
      outColor = vec4(1.0, 1.0, 0.8, 1.0);
   }else if(vertexColor == 4.0){
      outColor = vec4(1.0, 0.7, 0.7, 1.0);
   }else if(vertexColor == 5.0){
      outColor = vec4(0.4, 0.26, 0.13, 1.0);
   }else if(vertexColor == 6.0){
      outColor = vec4(0.33, 0.18, 0.05, 1.0);
   }else if(vertexColor == 7.0){
      outColor = vec4(0.7, 0.9, 1.0, 0.4);
   }else if(vertexColor == 8.0){
      outColor = vec4(0.1, 0.1, 0.1, 1.0);
   }else if(vertexColor == 9.0){
      outColor = vec4(0.65, 0.16, 0.16, 1.0);
   }else{
      outColor = vec4(-1.0, -1.0, -1.0, -1.0);
   }
}
