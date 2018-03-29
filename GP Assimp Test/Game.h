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

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2.h"

//#include "Input.h"

// bullet
#include "CollisionSystem.h"

//irrklang
#include "AudioSystem.h"

#define DEG_TO_RADIAN 0.017453293

class Game{
public:
	CollisionSystem* collisionSystem;
	AudioSystem* audioSystem;

	// Properties
	
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	// Camera
	Camera* camera;
	GLFWwindow *window = nullptr;

	// create the EntityFactory
	EntityFactory* _entFactory;

	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	glm::vec3 cameraModifier;

	//player & player camera positions
	glm::vec3 playerCameraPos;


	// default constructor
	Game();

	void Init(GLFWwindow* window);

	// constructor
	//Game(GLFWwindow* _window);

	// Start game
	int Play();

	// Moves/alters the camera positions based on user input
	void DoMovement() ;

	// Is called whenever a key is pressed/released via GLFW
	// Singleton is accessed via getInstance() for callback methods
	// from https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
	static inline  Game& getInstance(){
		static Game instance;
		return instance;
		
	}
	static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) ;

	static void MouseCallback(GLFWwindow *window, double xPos, double yPos) ;

};
