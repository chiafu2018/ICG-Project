#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aVertexColor;

out VS_OUT {
   vec2 TexCoord;
   vec4 outColor; 
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   vs_out.TexCoord = aTexCoord;
   

   // each vertex has its own color, defined in obj file. 
   // if that the object is gonna to bind the texture, we are going to set outColor to vec4(-1.0, -1.0, -1.0, -1.0)
   float vertexColor = aVertexColor; 

   vec4 outColor;

   if(vertexColor == 0.0){
      outColor = vec4(1.0, 1.0, 1.0, 1.0); 
   }else if(vertexColor == 1.0){
      outColor = vec4(0.0, 0.0, 0.0, 1.0); 
   }else if(vertexColor == 2.0){
      outColor = vec4(0.5, 0.5, 0.5, 1.0); 
   }else if(vertexColor == 3.0){
      outColor = vec4(0.0, 0.0, 0.0, 1.0); 
   }else if(vertexColor == 4.0){
      outColor = vec4(1.0, 1.0, 1.0, 1.0); 
   }else if(vertexColor == 5.0){
      outColor = vec4(0.0, 0.0, 0.0, 1.0); 
   }else if(vertexColor == 6.0){
      outColor = vec4(1.0, 1.0, 0.0, 1.0); 
   }else if(vertexColor == 7.0){
      outColor = vec4(0.0, 0.0, 0.0, 1.0); 
   }else if(vertexColor == 8.0){
      outColor = vec4(0.5, 0.5, 0.5, 1.0); 
   }else if(vertexColor == 9.0){
      outColor = vec4(0.0, 0.0, 0.0, 1.0); 
   }else{
      outColor = vec4(-1.0, -1.0, -1.0, -1.0);
   }
   vs_out.outColor = outColor;
}
