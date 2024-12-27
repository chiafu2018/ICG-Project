#include <bits/stdc++.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/cube.h"
#include "header/object.h"
#include "header/shader.h"
//#include "header/particle.h"
#include "header/stb_image.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int loadCubemap(std::vector<string> &mFileName);
unsigned int loadParticle(const std::string &filepath);

struct material_t{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float gloss;
};

struct light_t{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct model_t{
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    Object* object;
};

struct camera_t{
    glm::vec3 position;
    glm::vec3 up;
    float rotationY;
};

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// cube map 
unsigned int cubemapTexture;
unsigned int particleTexture;

unsigned int cubemapVAO, cubemapVBO;

// shader programs 
int shaderProgramIndex = 0;
std::vector<shader_program_t*> shaderPrograms;
shader_program_t* cubemapShader;
shader_program_t* particleShader;

// additional dependencies
light_t light;
material_t material;
camera_t camera;

model_t ae86;
model_t surface;
model_t palm;
model_t rock; 


// model matrix
int moveDir = -1;
glm::mat4 cameraModel;

glm::mat4 ae86Model;
glm::mat4 surfaceModel;
glm::mat4 palmModel;
glm::mat4 rockModel;

// scores 
unsigned int score = 0, lap= 0; 



const float CAMERA_FOLLOW_DISTANCE = 200.0f; // Distance behind the car
const float CAMERA_HEIGHT = 20.0f;          // Height above the car
const float CAMERA_SMOOTHING = 0.1f;        // Lower = smoother camera movement
const float CAR_MOVEMENT_SPEED = 0.1f;
// const float CAR_MOVEMENT_SPEED = 1.0f;

// const float CAR_MIN_Z = -20.0f; // car minimum for generating the smoke 
// const float CAR_MAX_Z = 675.0f;
const float CAR_MIN_Z = -29.0f; // car minimum for generating the smoke 
const float CAR_MAX_Z = 29.0f;

// const float CAR_MIN_X = -180.0f;
// const float CAR_MAX_X = 180.0f;

const float CAR_MIN_X = -2.0f;
const float CAR_MAX_X = 2.0f;


bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false, moveCameraRight = false, moveCameraLeft = false;




glm::vec3 smokeBasePosition = glm::vec3(0.0f, 0.0f, 70.0f); 


//// Particles 
struct ParticleCreateInfo {
	glm::vec3 position, velocity, acceleration, up_vector, side_vector;
	float lifetime;
	glm::vec3 color;
};

class Particle {
public:
	glm::vec3 position, velocity, acceleration, up_vector, side_vector;
	glm::mat4 modelTransform;
	float t, lifetime;
	glm::vec3 color;
	glm::vec4 tint;
	Particle(ParticleCreateInfo* createInfo);
	void update(float rate);
  float initialHeight;
  static constexpr float MAX_HEIGHT_DIFFERENCE = 2.0f; 
};


Particle::Particle(ParticleCreateInfo* createInfo) {

	this->position = createInfo->position;
	this->velocity = createInfo->velocity;
	this->acceleration = createInfo->acceleration;
  this->up_vector = createInfo->up_vector;
  this->side_vector = createInfo->side_vector; 

	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(modelTransform, position);

	this->color = createInfo->color;
	this->lifetime = createInfo->lifetime;
	t = 0.0f;
	initialHeight = position.y;

}



// At global scope
static std::default_random_engine generator(std::chrono::steady_clock::now().time_since_epoch().count());

float random() {
   return float(generator() % 1000) / 1000;
  //cout<< a <<endl;
  //return a;
}
// float random() {
//     unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
//     std::default_random_engine generator(seed);
//     return float(generator() % 1000) / 1000;
// }

// At global scope
// static std::default_random_engine generator(std::chrono::steady_clock::now().time_since_epoch().count());
// std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

// float random() {
//     float a = distribution(generator);
//     cout<< a <<endl;
//     return a;
// }

void Particle::update(float rate) {
	t += rate;
  float ageFactor = t / lifetime; 
 // float velocityScale = glm::mix(1.0f, 0.98f, ageFactor); // Interpolate between 1.0 and 0.8
  float spreadFactor = glm::mix(1.0f, 1.1f, ageFactor); // Increase spread as ageFactor grows
  //velocity.x *= spreadFactor;
  //velocity.y *= spreadFactor;
  //velocity *= velocityScale;
  //find the min velocity 
	//velocity += rate * acceleration;


	//float y = float(generator() % 1000)/10000;
  float y = glm::mix((random() + 0.2f)* 0.1f, random() * 0.01f, ageFactor);
  up_vector.y = y;
  
	//float x = float(generator() % 10)/10000;
  float x = glm::mix((random() - 0.5f)* 0.01f, (random() - 0.5f)* 0.1f, ageFactor);
  side_vector.x = x;
  // cout<<"lifetime "<<endl;
  // cout<<" x " << x << " y "<<y<<endl;
  velocity += up_vector + side_vector; 
	position += rate * velocity;
	// if (position.z < 0) {
	// 	velocity.z *= -0.5f
	// 	position.z = 0.0f;
	// }

	modelTransform = glm::mat4(1.0f);
  //cout<<position.x<<" "<< position.y<<" "<<position.z<<" "<<endl;
	modelTransform = glm::translate(modelTransform, position);


  float heightDiff = position.y - initialHeight;
  float heightFactor = glm::clamp(heightDiff / MAX_HEIGHT_DIFFERENCE, 0.0f, 1.0f);
 
  glm::vec3 lightColor = glm::vec3(0.9f, 0.9f, 0.9f);
  //glm::vec3 fadeColor = color + (glm::vec3(1.0f) - color) * ageFactor;
  glm::vec3 heightBlendedColor = glm::mix(color, lightColor, ageFactor);
  ageFactor *= 1.25;
	tint = glm::vec4(heightBlendedColor, 1.0f - ageFactor);

}

//Paricles variables
std::vector<Particle*> particles;



//////////////////////////////////////////////////////////////////////////
// Parameter setup, 
// You can change any of the settings if you want

void camera_setup(){
    camera.position = glm::vec3(0.0, 20.0, 200.0);
    camera.up = glm::vec3(0.0, 1.0, 0.0);
    // set the camera angle to see the car from starting line
    // camera.rotationY = 210;
    camera.rotationY = 0;

}

void light_setup(){
    light.position = glm::vec3(0.0, 1000.0, 0.0);
    light.ambient = glm::vec3(1.0);
    light.diffuse = glm::vec3(1.0);
    light.specular = glm::vec3(1.0);
}

void material_setup(){
    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(1.0);
    material.specular = glm::vec3(0.7);
    material.gloss = 10.5;
}

// void particle_setup(){
//     exhaustSmoke = new ParticleSystem(500);
//   for(int i = 0; i < 4; i++) {
//       wheelSmoke.push_back(new ParticleSystem(200));
// }
// }
//////////////////////////////////////////////////////////////////////////

// void model_setup(){

// // Load the object and texture for each model here 

void model_setup(){

// Load the object and texture for each model here 

#if defined(__linux__) || defined(__APPLE__)
    std::string objDir = "../../src/asset/obj/";
    std::string textureDir = "../../src/asset/texture/";
#else
    std::string objDir = "..\\..\\src\\asset\\obj\\";
    std::string textureDir = "..\\..\\src\\asset\\texture\\";
#endif
    ae86Model = glm::mat4(1.0f);
    surfaceModel = glm::mat4(1.0f);
    palmModel = glm::mat4(1.0f);
    rockModel = glm::mat4(1.0f);
    // starting line is at 0, 0, 29
    // ae86.position = glm::vec3(0.0f, 0.0f, 29.0f);
    ae86.position = glm::vec3(0.0f, 0.0f, 0.0f);

    ae86.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    ae86.object = new Object(objDir + "ae86.obj");

    ae86.scale = glm::vec3(30.0f, 30.0f, 30.0f);
    ae86.object->load_to_buffer();

    surface.position = glm::vec3(0.0f, -13.0f, 0.0f);  // Position below the car
    surface.scale = glm::vec3(50.0f, 1.0f, 700.0f);  // Large plane surface
    surface.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    surface.object = new Object(objDir + "road.obj");

    surface.object->load_to_buffer();
    surface.object->load_texture(textureDir + "road.jpg");

    // Load palm tree model 
    palm.position = glm::vec3(700.0f, -13.0f, 0.0f); 
    palm.scale = glm::vec3(0.1f, 0.1f, 0.1f); 
    palm.rotation = glm::vec3(1.0f, 0.0f, 0.0f); 
    palm.object = new Object(objDir + "palm.obj");

    palm.object->load_to_buffer(); 
    palm.object->load_texture(textureDir + "palm.jpg");


    rock.position = glm::vec3(0.0f, 20.0f, 0.0f);  // Position below the car
    rock.scale = glm::vec3(0.5f, 0.5f, 0.1f);  // Large plane surface
    rock.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    rock.object = new Object(objDir + "rock.obj");

    rock.object->load_to_buffer();
    rock.object->load_texture(textureDir + "rock.jpg");

}


void shader_setup(){

// Setup the shader program for each shading method

#if defined(__linux__) || defined(__APPLE__)
    std::string shaderDir = "../../src/shaders/";
#else
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    std::vector<std::string> shadingMethod = {
        "default",                              // default shading
        // "bling-phong", "gouraud", "metallic",   // addional shading effects (basic)
        // "glass_schlick", "glass_empricial",     // addional shading effects (advanced)
    };

    for(int i=0; i<shadingMethod.size(); i++){
        std::string vpath = shaderDir + shadingMethod[i] + ".vert";
        std::string gpath = shaderDir + shadingMethod[i] + ".geom";
        std::string fpath = shaderDir + shadingMethod[i] + ".frag";

        shader_program_t* shaderProgram = new shader_program_t();
        shaderProgram->create();
        shaderProgram->add_shader(vpath, GL_VERTEX_SHADER);
        shaderProgram->add_shader(gpath, GL_GEOMETRY_SHADER);
        shaderProgram->add_shader(fpath, GL_FRAGMENT_SHADER);
        shaderProgram->link_shader();
        shaderPrograms.push_back(shaderProgram);
    } 

}

void particleSetup(){
    // std::string particleDir = "..\\..\\src\\asset\\texture\\smoke.jpg";
    std::string shaderDir = "..\\..\\src\\shaders\\";

    std::string vpath = shaderDir + "particle.vert";
    std::string gpath = shaderDir + "particle.geom";
    std::string fpath = shaderDir + "particle.frag";
    // particleTexture = loadParticle(particleDir);
    
    particleShader = new shader_program_t();
    particleShader->create();
    particleShader->add_shader(vpath, GL_VERTEX_SHADER);
    particleShader->add_shader(gpath, GL_GEOMETRY_SHADER);
    particleShader->add_shader(fpath, GL_FRAGMENT_SHADER);
    particleShader->link_shader();

}


void cubemap_setup(){

// Setup all the necessary things for cubemap rendering
// Including: cubemap texture, shader program, VAO, VBO

#if defined(__linux__) || defined(__APPLE__)
    std::string cubemapDir = "../../src/asset/texture/skybox/";
    std::string shaderDir = "../../src/shaders/";
#else
    std::string cubemapDir = "..\\..\\src\\asset\\texture\\skybox\\";
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    // setup texture for cubemap
    std::vector<std::string> faces
    {
        cubemapDir + "right.jpg",
        cubemapDir + "left.jpg",
        cubemapDir + "top.jpg",
        cubemapDir + "bottom.jpg",
        cubemapDir + "front.jpg",
        cubemapDir + "back.jpg"
    };
    cubemapTexture = loadCubemap(faces);   

    // setup shader for cubemap
    std::string vpath = shaderDir + "cubemap.vert";
    std::string fpath = shaderDir + "cubemap.frag";
    
    cubemapShader = new shader_program_t();
    cubemapShader->create();
    cubemapShader->add_shader(vpath, GL_VERTEX_SHADER);
    cubemapShader->add_shader(fpath, GL_FRAGMENT_SHADER);
    cubemapShader->link_shader();

    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void setup(){

    // Initialize shader model camera light material
    light_setup();
    model_setup();
    shader_setup();
    camera_setup();
    cubemap_setup();
    particleSetup();


    // Enable depth test, face culling ...
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void makeParticles(glm::vec3 position, glm::vec3 incident, glm::vec3 normal) {

	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	ParticleCreateInfo createInfo;
	//createInfo.acceleration = glm::vec3(0, 0, -0.001);
	//createInfo.acceleration = glm::vec3(0.009, 0.009, 0.0);
	createInfo.color = glm::vec3(0.4, 0.4, 0.4); 
	createInfo.position = position;

	for (int i = 0; i < 500; ++i) {
		float x = float(generator() % 100) / 50.f - 1.0f;
		float y = float(generator() % 100) / 50.f - 1.0f;
		float z = float(generator() % 100) / 50.f - 1.0f;



		glm::vec3 randomization = glm::vec3(x, y, z);
		glm::vec3 randomizedNormal = glm::normalize(0.9f * randomization + normal);

		x = float(generator() % 100) / 10000.0f;
		glm::vec3 outgoing = x * glm::reflect(incident, randomizedNormal);

		createInfo.velocity = outgoing;

		x = 20.0f + float(generator() % 100)/5.0f;
		createInfo.lifetime = x;

		particles.push_back(new Particle(&createInfo));
	}
}

void gameLogic(){
    // add score 
    score += 100;
    lap++; 
    std::cout << "Lap:" << lap << " Score:" << score << std::endl; 

    // change the rock position 
    int choice = rand()%3;
    if(choice == 0){
        rock.position.x = -130;
    }else if(choice == 1){
        rock.position.x = 0;
    }else if(choice == 2){
        rock.position.x = 130;
    }
}

void checkCarCollision(){
    // check if the user hit the rock

    // std::cout<<"Ae86: \n"; 
    // std::cout<< ae86.position.x <<" "<< ae86.position.y <<" "<< ae86.position.z <<std::endl; 
    // std::cout<<"Rock: \n"; 
    // std::cout<< rock.position.x <<" "<< rock.position.y <<" "<< rock.position.z <<std::endl; 



    if(ae86.position.x*65 == rock.position.x && ae86.position.z+2 == rock.position.z && (moveForward||moveBackward)){
        if(score>200){
            score -= 200;
        }else{
            score = 0;
        }
        std::cout << "You hit the rock! " << " Score:" << score << std::endl; 
    }
}


void carMovementLogic(){
    if (moveForward) {
        ae86.position.z -= CAR_MOVEMENT_SPEED;
        smokeBasePosition.z -= CAR_MOVEMENT_SPEED * ae86.scale.z;    
        
        if (ae86.position.z <= CAR_MIN_Z) {
            ae86.position.z = CAR_MAX_Z;
            gameLogic(); 
        }
        else {
        camera.position.z -= CAR_MOVEMENT_SPEED * ae86.scale.z;
        }
    }

    if (moveBackward) {
        ae86.position.z += CAR_MOVEMENT_SPEED;
        smokeBasePosition.z += CAR_MOVEMENT_SPEED * ae86.scale.z;    
        
        if (ae86.position.z >= CAR_MAX_Z) {
            ae86.position.z = CAR_MAX_Z;
        }
        else {
        camera.position.z += CAR_MOVEMENT_SPEED * ae86.scale.z;
        }
    }

    // if (moveLeft) {
    //     ae86.position.x -= CAR_MOVEMENT_SPEED*2;
    //     smokeBasePosition.x -= CAR_MOVEMENT_SPEED * ae86.scale.x*2;    
        
    //     if (ae86.position.z <= CAR_MIN_X) {
    //         ae86.position.z = CAR_MIN_X;
    //     }
    //     else {
    //     camera.position.x <= CAR_MOVEMENT_SPEED * ae86.scale.x;
    //     }
    // }

    // if (moveRight) {
    //     ae86.position.x += CAR_MOVEMENT_SPEED*2;
    //     smokeBasePosition.x += CAR_MOVEMENT_SPEED * ae86.scale.x*2;    
        
    //     if (ae86.position.z >= CAR_MAX_X) {
    //         ae86.position.z = CAR_MIN_X;
    //     }
    //     else {
    //     camera.position.x >= CAR_MOVEMENT_SPEED * ae86.scale.x;
    //     }
    // }

    if (moveLeft) {
        ae86.position.x -= CAR_MOVEMENT_SPEED;
        smokeBasePosition.x -= CAR_MOVEMENT_SPEED * ae86.scale.x;    
        
        if (ae86.position.x <= -2) {
            ae86.position.x = -2;
        }
        else {
            camera.position.x <= CAR_MOVEMENT_SPEED * ae86.scale.x;
        }
    }

    if (moveRight) {
        ae86.position.x += CAR_MOVEMENT_SPEED;
        smokeBasePosition.x += CAR_MOVEMENT_SPEED * ae86.scale.x;    
        
        if (ae86.position.x >= 2) {
            ae86.position.x = 2;
        }
        else {
        camera.position.x >= CAR_MOVEMENT_SPEED * ae86.scale.x;
        }
    }


    checkCarCollision(); 
}





void update(){

    ae86Model = glm::mat4(1.0f);
    ae86Model = glm::scale(ae86Model, ae86.scale);
    ae86Model = glm::translate(ae86Model, ae86.position);

    surfaceModel = glm::mat4(1.0f);
    surfaceModel = glm::scale(surfaceModel, surface.scale);
    surfaceModel = glm::translate(surfaceModel, surface.position);

    rockModel = glm::mat4(1.0f);
    rockModel = glm::scale(rockModel, rock.scale);
    //randomly generate the rock position later 
    rockModel = glm::translate(rockModel, rock.position);


    // camera.rotationY = (camera.rotationY > 360.0) ? 0.0 : camera.rotationY;
    // cameraModel = glm::mat4(1.0f);
    // cameraModel = glm::rotate(cameraModel, glm::radians(camera.rotationY), camera.up);
    // cameraModel = glm::translate(cameraModel, camera.position);

    // udpate
    std::this_thread::sleep_for(std::chrono::milliseconds(20));  // Delay of 10 ms

    carMovementLogic(); 
    //updateCameraPosition();
    camera.rotationY = (camera.rotationY > 360.0) ? 0.0 : camera.rotationY;
    cameraModel = glm::mat4(1.0f);
    cameraModel = glm::rotate(cameraModel, glm::radians(camera.rotationY), camera.up);
    cameraModel = glm::translate(cameraModel, camera.position);
    //float currentTime = glfwGetTime(); // Get current time
    if(moveForward)
        makeParticles(smokeBasePosition, glm::normalize(glm::vec3(1, 0, 1)), glm::vec3(0, 1, 0));


	for (int i = 0; i < particles.size(); ++i) {

		Particle* particle = particles[i];
		particle->update(1.0f);

		if (particle->t >= particle->lifetime) {
			delete particle;
			particles.erase(particles.begin() + i--);
		}

	}
}

// Render AE86 car
void renderAE86(shader_program_t* shader) {
    shader->set_uniform_value("model", ae86Model);

    // apply neon effect
    shader->set_uniform_value("useNeonEffect", false);

    ae86.object->render();
}

// Render the surface (plane)
void renderSurface(shader_program_t* shader) {
    shader->set_uniform_value("model", surfaceModel);

    // apply neon effect
    shader->set_uniform_value("useNeonEffect", true);

    surface.object->render();
}

// Render palms along the road
void renderPalms(shader_program_t* shader) {
    float spacing = 100.0f;
    int numPalms = 10;
    float roadOffset = 130.0f;

    for (int i = 0; i < numPalms; i++) {
        // Left side
        palmModel = glm::mat4(1.0f);
        palmModel = glm::translate(palmModel, glm::vec3(-roadOffset, -13.0f, -300.0f + i * spacing));
        palmModel = glm::scale(palmModel, palm.scale);
        palmModel = glm::rotate(palmModel, glm::radians(-90.0f), palm.rotation);
        shader->set_uniform_value("model", palmModel);

        // apply neon effect
        shader->set_uniform_value("useNeonEffect", true);

        palm.object->render();

        // Right side
        palmModel = glm::mat4(1.0f);
        palmModel = glm::translate(palmModel, glm::vec3(roadOffset, -13.0f, -300.0f + i * spacing));
        palmModel = glm::scale(palmModel, palm.scale);
        palmModel = glm::rotate(palmModel, glm::radians(-90.0f), palm.rotation);
        shader->set_uniform_value("model", palmModel);

        // apply neon effect
        shader->set_uniform_value("useNeonEffect", true);

        palm.object->render();
    }
}

// Render the skybox
void renderSkybox(const glm::mat4& view, const glm::mat4& projection) {
    glDepthMask(GL_FALSE);
    cubemapShader->use();
    glm::mat4 skyboxview = glm::mat4(glm::mat3(view));
    cubemapShader->set_uniform_value("skyboxview", skyboxview);
    cubemapShader->set_uniform_value("projection", projection);
    glBindVertexArray(cubemapVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}


// Render the surface (plane)
void renderRock(shader_program_t* shader) {
    shader->set_uniform_value("model", rockModel);

    // apply neon effect
    shader->set_uniform_value("useNeonEffect", false);

    rock.object->render();
}


void render(){

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate view, projection matrix
    glm::mat4 view = glm::lookAt(glm::vec3(cameraModel[3]), glm::vec3(ae86.position), camera.up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

    // Set matrix for view, projection, model transformation
    shaderPrograms[shaderProgramIndex]->use();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("view", view);
    shaderPrograms[shaderProgramIndex]->set_uniform_value("projection", projection);
    

    if(shaderProgramIndex == 1 || shaderProgramIndex == 2){
        shaderPrograms[shaderProgramIndex]->set_uniform_value("material_ambient", material.ambient);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("material_diffuse", material.diffuse);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("material_specular", material.specular);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("material_gloss", material.gloss);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("light_ambient", light.ambient);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("light_diffuse", light.diffuse);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("light_specular", light.specular);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("light_position", light.position);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("camera_position", cameraModel[3]);
    }else if(shaderProgramIndex == 3 || shaderProgramIndex == 4 || shaderProgramIndex == 5){
        shaderPrograms[shaderProgramIndex]->set_uniform_value("light_position", light.position);
        shaderPrograms[shaderProgramIndex]->set_uniform_value("camera_position", cameraModel[3]);
    }

    // shaderPrograms[shaderProgramIndex]->set_uniform_value("time", float(glfwGetTime()));


    renderAE86(shaderPrograms[shaderProgramIndex]);
    renderSurface(shaderPrograms[shaderProgramIndex]);
    renderPalms(shaderPrograms[shaderProgramIndex]);
    renderRock(shaderPrograms[shaderProgramIndex]);
    renderSkybox(view, projection);


    // // Enable blending
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    particleShader->use();

    for (Particle* particle : particles) {

        particleShader->set_uniform_value("model", particle->modelTransform);
        particleShader->set_uniform_value("view", view);
        particleShader->set_uniform_value("projection", projection);
        particleShader->set_uniform_value("tint", particle->tint.a);
        particleShader->set_uniform_value("my_color", glm::vec3(particle->tint.r, particle->tint.g, particle->tint.b)); 
      // glActiveTexture(GL_TEXTURE0);
       //glBindTexture(GL_TEXTURE_2D, particleTexture);
       //particleShader->set_uniform_value("particleTexture", 0);
        glDrawArrays(GL_POINTS, 0, 1);
      //glBindTexture(GL_TEXTURE_2D, 0);
     // glBindVertexArray(0);
    }
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);


}




int main() {
    
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ICG-Final-Project", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup texture, model, shader ...e.t.c
    setup();
    
    // Render loop, main logic can be found in update, render function
    while (!glfwWindowShouldClose(window)) {
        update(); 
        render(); 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



// Add key callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
    // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key press.
    // Most keys will also emit events with GLFW_REPEAT actions while a key is held down.
    // https://www.glfw.org/docs/3.3/input_guide.html

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // shader program selection
    if (key == GLFW_KEY_0 && (action == GLFW_REPEAT || action == GLFW_PRESS)) 
        shaderProgramIndex = 0;
    if (key == GLFW_KEY_1 && (action == GLFW_REPEAT || action == GLFW_PRESS)) 
        shaderProgramIndex = 1;
    if (key == GLFW_KEY_2 && (action == GLFW_REPEAT || action == GLFW_PRESS)) 
        shaderProgramIndex = 2;
    if (key == GLFW_KEY_3 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 3;
    if (key == GLFW_KEY_4 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 4;
    if (key == GLFW_KEY_5 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 5;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W) moveForward = true;
        if (key == GLFW_KEY_S) moveBackward = true;
        if (key == GLFW_KEY_A) moveLeft = true;
        if (key == GLFW_KEY_D) moveRight = true;
        if (key == GLFW_KEY_LEFT) moveCameraLeft = true;
        if(key == GLFW_KEY_RIGHT) moveCameraRight = true;
    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W) moveForward = false;
        if (key == GLFW_KEY_S) moveBackward = false;
        if (key == GLFW_KEY_A) moveLeft = false;
        if (key == GLFW_KEY_D) moveRight = false;
        if (key == GLFW_KEY_LEFT) moveCameraLeft = false;
        if(key == GLFW_KEY_RIGHT) moveCameraRight =false;
    }

    // float carSpeed = 1.0f; 

    // if (moveForward) {
    //     ae86.position.z -= carSpeed;
    //     if(ae86.position.z < -29){
    //         ae86.position.z = 29;
    //         // add score 
    //         score += 100;
    //         lap++; 
    //         std::cout << "Lap:" << lap << " Score:" << score << std::endl; 

    //         // change the rock position 
    //         int choice = rand()%3;
    //         if(choice == 0){
    //             rock.position.x = -130;
    //         }else if(choice == 1){
    //             rock.position.x = 0;
    //         }else if(choice == 2){
    //             rock.position.x = 130;
    //         }

    //     }
    // }

    // if (moveBackward) {
    //     ae86.position.z += carSpeed;
    //     if(ae86.position.z > 29){
    //         ae86.position.z = 29;
    //     }
    // }
    
    // // there is only two lanes in the road -2, 0, 2
    // if (moveLeft) {
    //     ae86.position.x -= carSpeed*2;
    //     if(ae86.position.x < -2){
    //         ae86.position.x = -2;
    //     } 
    // }

    // // there is only two lanes in the road -2, 0, 2
    // if (moveRight) {
    //     ae86.position.x += carSpeed*2;
    //     if(ae86.position.x > 2){
    //         ae86.position.x = 2;
    //     }
    // }

    // if (moveCameraLeft) {
    //     camera.rotationY -= 10.0;
    // }

    // if (moveCameraRight) {
    //     camera.rotationY += 10.0;  
    // }
    
    // // check if the user hit the rock
    // if(ae86.position.x*65 == rock.position.x && ae86.position.z-2 == rock.position.z && (moveForward||moveBackward)){
    //     if(score>200){
    //         score -= 200;
    //     }else{
    //         score = 0;
    //     }
    //     std::cout << "You hit the rock! " << " Score:" << score << std::endl; 
    // }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// Loading cubemap texture
unsigned int loadCubemap(vector<std::string>& faces){

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}  


unsigned int loadParticle(const std::string &filepath) {
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    
    // //Set texture parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true);
    // unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0); // Force load with alpha

    // if (data) {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //     std::cout << "Texture loaded successfully. Size: " << width << "x" << height << " Channels: " << nrChannels << std::endl;
    // } else {
    //     std::cout << "Failed to load texture at path: " << filepath << std::endl;
    // }

    // // glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    // // glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
    // // glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // // glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // // glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // // glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // stbi_image_free(data);
    // return texture;
    unsigned int texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        std::cout << "Failed to load texture1" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}