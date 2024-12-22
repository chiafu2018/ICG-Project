#version 330 core

// TODO 3:
// Implement Gouraud shading

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 material_ambient;
uniform vec3 material_diffuse; 
uniform vec3 material_specular; 
uniform float material_gloss; 

uniform vec3 light_position; 
uniform vec3 light_ambient; 
uniform vec3 light_diffuse; 
uniform vec3 light_specular; 
uniform vec3 camera_position;

out vec2 TexCoord;  
out vec3 I;
out vec4 testPosition;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

    vec4 testPosition = gl_Position; 

	TexCoord = aTexCoord;

    vec3 world_position, light_dir, view_dir, halfway_vector, normal; 

    world_position = vec3(model * vec4(aPos, 1.0));
    light_dir = normalize(light_position - world_position); 
    view_dir = normalize(camera_position - world_position); 

    halfway_vector = normalize(light_dir + view_dir);

    normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    vec3 Ia, Id, Is;

    // Ambient 
    Ia = light_ambient * material_ambient; 
    
    // Diffuse
    if(dot(light_dir, normal) > 0.0){
        Id = light_diffuse * material_diffuse * dot(light_dir, normal);
    }else{
        Id = vec3(0.0); 
    }

    // Specular
    if(dot(normal, halfway_vector) > 0.0){
        Is = light_specular * material_specular * pow(dot(normal, halfway_vector), material_gloss); 
    }else{
        Is = vec3(0.0); 
    }
    
    I = Ia + Id + Is;
}