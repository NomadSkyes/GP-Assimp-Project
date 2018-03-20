// main.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

// Std.Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Entity.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2.h"

//#include "Input.h"

// bullet
#include "CollisionSystem.h"
CollisionSystem* collisionSystem;

//irrklang
#include "AudioSystem.h"
AudioSystem* audioSystem;

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

#define DEG_TO_RADIAN 0.017453293

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// entities
Entity playerEntity(glm::vec3(0.0f, 1.0f, 6.0f));
Entity level01Entity(glm::vec3(0.0f, 0.0f, 0.0f));
Entity drone01Entity(glm::vec3(0.0f, 0.0f, 0.0f));

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 cameraModifier(0.0f, 0.0f, 0.0f);

//Level Model Positions
glm::vec3 lvl1_aPos(0.0f, 0.0f, 0.0f);
glm::vec3 lvl1_bPos(16.0f, 0.0f, 0.0f);
glm::vec3 lvl1_cPos(16.0f, 0.0f, -16.0f);
glm::vec3 lvl1_dPos(0.0f, 0.0f, -16.0f);


//player & player camera positions
glm::vec3 playerCameraPos;


int main()
{
	// init irrklang
	audioSystem = new AudioSystem;

	// init bullet physics
	collisionSystem = new CollisionSystem();

	

	// give the entities a reference to the collision system
	playerEntity.SetCollisionSystem(collisionSystem);
	level01Entity.SetCollisionSystem(collisionSystem);
	drone01Entity.SetCollisionSystem(collisionSystem);

	// give the player entity a reference to the audio system
	playerEntity.SetAudioSystem(audioSystem);


	// set the listener's position to the camera's position
	audioSystem->UpdateListnerPosition(camera.GetPosition(), camera.GetFront());

	// give the player entity a sound
	playerEntity.setSound("res/audio/explosion.wav");

	// player create a rigidbody
	playerEntity.AddRigidBody(0);

	//adds cylinder rigidbody (1)
	drone01Entity.AddRigidBody(1);

	// pass the collision system into the level object to create a rigidbody
	level01Entity.AddMultipleRigidBodies(0.5, 5, 15, -7.5, 0, 0, 0.0);
	level01Entity.AddMultipleRigidBodies(0.5, 5, 15, 7.5, 0, 0, 0.0);
	level01Entity.AddMultipleRigidBodies(15, 5, 0.5, 0, 0, -7.5, 0.0);
	level01Entity.AddMultipleRigidBodies(15, 5, 0.5, 0, 0, 7.5, 0.0);
	level01Entity.AddMultipleRigidBodies(2.5, 5, 2.5, 0, 0, 0, 0.0);

	// add collision plane
	btRigidBody* plane = collisionSystem->AddPlane();

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	//GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Super Battle Arena!", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

	// Load models
	// set entity models 
	level01Entity.SetModel("res/models/ForestFloor.obj");
	playerEntity.SetModel("res/models/Gladiator.obj");
	drone01Entity.SetModel("res/models/Drone.obj");

	camera.SetAngle(-65.0f, -90.0f);

	// Draw in wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		//fixes camera on player
		playerCameraPos.x = playerEntity.GetPosition().x + cameraModifier.x;
		playerCameraPos.y = 10.0f;
		playerCameraPos.z = playerEntity.GetPosition().z + 5 + cameraModifier.z;
		camera.SetPosition(playerCameraPos);
		
		// set the listener's position to the camera's position
		audioSystem->UpdateListnerPosition(camera.GetPosition(), camera.GetFront());

		playerEntity.myTickCallback(collisionSystem->getWorld(),1);

		//collision
		collisionSystem->StepSimulation(1 / 60.0);	// step the physics simulation (default 1/60 seconds)


		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model_a;

		glm::mat4 enemy_1;

		glm::mat4 player_1;

		model_a = glm::translate(model_a, lvl1_aPos); // Translate it down a bit so it's at the center of the scene
		model_a = glm::scale(model_a, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		//model_a = glm::rotate(model_a, float(-90 * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_a));
		//lvl1_a.Draw(shader);
		level01Entity.GetModel().Draw(shader);

		enemy_1 = glm::translate(enemy_1, drone01Entity.GetPosition()); // Translate it down a bit so it's at the center of the scene
		enemy_1 = glm::scale(enemy_1, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		enemy_1 = glm::rotate(enemy_1, float(-90 * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		enemy_1 = glm::rotate(enemy_1, float(-drone01Entity.getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(enemy_1));
		drone01Entity.GetModel().Draw(shader);
		drone01Entity.LookAt(playerEntity.GetPosition());
		

		player_1 = glm::translate(player_1, playerEntity.GetPosition()); // Translate it down a bit so it's at the center of the scene
		player_1 = glm::scale(player_1, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
		player_1 = glm::rotate(player_1, float(playerEntity.getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(player_1));
		//player.Draw(shader);
		playerEntity.GetModel().Draw(shader);


		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_D])
	{
		playerEntity.Stop();
	}

	if (keys[GLFW_KEY_W])
	{
		playerEntity.ProcessKeyboard(ENTITY_UP, deltaTime);
	}
		

	if (keys[GLFW_KEY_S])
	{
		playerEntity.ProcessKeyboard(ENTITY_DOWN, deltaTime);
	}

	if (keys[GLFW_KEY_A])
	{
		playerEntity.ProcessKeyboard(ENTITY_LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D])
	{
		playerEntity.ProcessKeyboard(ENTITY_RIGHT, deltaTime);

	}

	if (keys[GLFW_KEY_SPACE])
	{
		// play sound
		playerEntity.playSound();
		playerEntity.Attack();
	}

	if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT])
	{
		if (keys[GLFW_KEY_UP])
		{
			playerEntity.SetAngle(180.0f);
			if (cameraModifier.z > -1.0f)
			{
				cameraModifier.z -= 0.1f;
			}
		}

		if (keys[GLFW_KEY_DOWN])
		{
			playerEntity.SetAngle(0.0f);
			if (cameraModifier.z < 1.0f)
			{
				cameraModifier.z += 0.1f;
			}
		}

		if (keys[GLFW_KEY_LEFT])
		{
			playerEntity.SetAngle(-90.0f);
			if (cameraModifier.x > -1.0f)
			{
				cameraModifier.x -= 0.1f;
			}
		}

		if (keys[GLFW_KEY_RIGHT])
		{
			playerEntity.SetAngle(90.0f);
			if (cameraModifier.x < 1.0f)
			{
				cameraModifier.x += 0.1f;
			}
		}

		if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_LEFT])
		{
			playerEntity.SetAngle(180.0f + 45.0f);

		}

		if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_LEFT])
		{
			playerEntity.SetAngle(0.0f - 45.0f);
		}

		if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_RIGHT])
		{
			playerEntity.SetAngle(180.0f - 45.0f);
		}

		if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_RIGHT])
		{
			playerEntity.SetAngle(0.0f + 45.0f);
		}

	}
	else
	{
		if (cameraModifier.x > 0)
			cameraModifier.x -= 0.1f;
		if (cameraModifier.x < 0)
			cameraModifier.x += 0.1f;
		if (cameraModifier.z > 0)
			cameraModifier.z -= 0.1f;
		if (cameraModifier.z < 0)
			cameraModifier.z += 0.1f;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}



void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	//camera.ProcessMouseMovement(xOffset, yOffset);
}
