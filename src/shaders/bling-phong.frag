#version 330 core

// TODO 2
// Implement Bling-Phong shading

out vec4 FragColor;

in vec2 TexCoord; 
in vec3 world_normal; 
in vec3 world_position; 

uniform sampler2D ourTexture;

uniform vec3 material_ambient;
uniform vec3 material_diffuse; 
uniform vec3 material_specular; 
uniform float material_gloss; 

uniform vec3 light_position; 
uniform vec3 light_ambient; 
uniform vec3 light_diffuse; 
uniform vec3 light_specular; 
uniform vec3 camera_position;

void main()
{
    vec3 light_dir, view_dir, halfway_vector;      
    
    light_dir = normalize(light_position - world_position); 
    view_dir = normalize(camera_position - world_position); 
    halfway_vector = normalize(light_dir + view_dir);

    vec3 Ia, Id, Is, I;

    // Ambient 
    Ia = light_ambient * material_ambient; 
    
    // Diffuse 
    if(dot(light_dir, world_normal) > 0.0){
        Id = light_diffuse * material_diffuse * dot(light_dir, world_normal);
    }else{
        Id = vec3(0.0); 
    }

    // specular
    if(dot(world_normal, halfway_vector) > 0.0){
        Is = light_specular * material_specular * pow(dot(world_normal, halfway_vector), material_gloss); 
    }else{
        Is = vec3(0.0); 
    }
    
    I = Ia + Id + Is;

    FragColor = texture(ourTexture, TexCoord) * vec4(I, 1.0);
}