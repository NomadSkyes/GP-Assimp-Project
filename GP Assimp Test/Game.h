#pragma once
// Std.Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// other
#include "Entity.h"
#include "Model.h"
#include "EntityFactory.h"
#include "GameManager.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2.h"

// bullet
#include "CollisionSystem.h"

//irrklang
#include "AudioSystem.h"

#define DEG_TO_RADIAN 0.017453293




class Game{
public:
	CollisionSystem* collisionSystem = nullptr;
	AudioSystem* audioSystem = nullptr;
	GameManager* _gm = nullptr;

	// Properties
	
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	// Camera
	Camera* camera;
	GLFWwindow *window = nullptr;


	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	glm::vec3 cameraModifier;

	//player & player camera positions
	glm::vec3 playerCameraPos;

	// rendering
	// Setup and compile our shaders
	Shader* shader;
	glm::mat4 projection;

	// default constructor
	Game();

	int Init(GLFWwindow* window);

	

	// Start game
	int Play();

	// Moves/alters the camera positions based on user input
	void DoMovement() ;

	void Update(double deltaTime);
	void Render();
	
	static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) ;

	static void MouseCallback(GLFWwindow *window, double xPos, double yPos) ;

};
