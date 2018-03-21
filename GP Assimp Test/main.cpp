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

// create the EntityFactory
EntityFactory* _entFactory;

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 cameraModifier(0.0f, 0.0f, 0.0f);

//player & player camera positions
glm::vec3 playerCameraPos;


int main()
{
	// init irrklang
	audioSystem = new AudioSystem;

	// init bullet physics
	collisionSystem = new CollisionSystem();

	// create the EntityFactory
	_entFactory = new EntityFactory();
	_entFactory->SetUp(audioSystem, collisionSystem);


	// set the listener's position to the camera's position
	audioSystem->UpdateListnerPosition(camera.GetPosition(), camera.GetFront());


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


	// create the entities
	_entFactory->CreateEntity(0);
	_entFactory->CreateEntity(1);
	_entFactory->CreateEntity(2);
	_entFactory->CreateEntity(3);


	camera.SetAngle(-65.0f, -90.0f);

	// Draw in wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		//fixes camera on player
		playerCameraPos.x = _entFactory->GetPlayer().GetPosition().x + cameraModifier.x;
		playerCameraPos.y = 10.0f;
		playerCameraPos.z = _entFactory->GetPlayer().GetPosition().z + 5 + cameraModifier.z;
		camera.SetPosition(playerCameraPos);
		
		// set the listener's position to the camera's position
		audioSystem->UpdateListnerPosition(camera.GetPosition(), camera.GetFront());

		_entFactory->GetPlayer().myTickCallback(collisionSystem->getWorld(),1);

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
		glm::mat4 model_b;

		glm::mat4 enemy_1;

		glm::mat4 player_1;

		

		model_a = glm::translate(model_a, _entFactory->GetLevel_01().GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_a = glm::scale(model_a, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		//model_a = glm::rotate(model_a, float(-90 * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_a));
		//lvl1_a.Draw(shader);
		_entFactory->GetLevel_01().GetModel().Draw(shader);

		model_b = glm::translate(model_b, _entFactory->GetLevel_02().GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_b = glm::scale(model_b, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_b));
		_entFactory->GetLevel_02().GetModel().Draw(shader);

		
		enemy_1 = glm::translate(enemy_1, _entFactory->GetDrone().GetPosition()); // Translate it down a bit so it's at the center of the scene
		enemy_1 = glm::scale(enemy_1, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		enemy_1 = glm::rotate(enemy_1, float(-90 * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		enemy_1 = glm::rotate(enemy_1, float(-_entFactory->GetDrone().getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(enemy_1));
		_entFactory->GetDrone().GetModel().Draw(shader);
		_entFactory->GetDrone().LookAt(_entFactory->GetPlayer().GetPosition());
		
		if (_entFactory->GetPlayer().IsAlive()) {
			player_1 = glm::translate(player_1, _entFactory->GetPlayer().GetPosition()); // Translate it down a bit so it's at the center of the scene
			player_1 = glm::scale(player_1, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
			player_1 = glm::rotate(player_1, float(_entFactory->GetPlayer().getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(player_1));
			//player.Draw(shader);
			_entFactory->GetPlayer().GetModel().Draw(shader);
		}

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
		_entFactory->GetPlayer().Stop();
	}

	if (keys[GLFW_KEY_W])
	{
		_entFactory->GetPlayer().ProcessKeyboard(ENTITY_UP, deltaTime);
	}
		

	if (keys[GLFW_KEY_S])
	{
		_entFactory->GetPlayer().ProcessKeyboard(ENTITY_DOWN, deltaTime);
	}

	if (keys[GLFW_KEY_A])
	{
		_entFactory->GetPlayer().ProcessKeyboard(ENTITY_LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D])
	{
		_entFactory->GetPlayer().ProcessKeyboard(ENTITY_RIGHT, deltaTime);

	}

	if (keys[GLFW_KEY_SPACE])
	{
		// play sound
		_entFactory->GetPlayer().playSound();
		_entFactory->GetPlayer().Attack();
	}
	if (keys[GLFW_KEY_ENTER])
	{
		// kill player
		_entFactory->GetPlayer().Kill();
	}
	

	if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT])
	{
		if (keys[GLFW_KEY_UP])
		{
			_entFactory->GetPlayer().SetAngle(180.0f);
			if (cameraModifier.z > -1.0f)
			{
				cameraModifier.z -= 0.1f;
			}
		}

		if (keys[GLFW_KEY_DOWN])
		{
			_entFactory->GetPlayer().SetAngle(0.0f);
			if (cameraModifier.z < 1.0f)
			{
				cameraModifier.z += 0.1f;
			}
		}

		if (keys[GLFW_KEY_LEFT])
		{
			_entFactory->GetPlayer().SetAngle(-90.0f);
			if (cameraModifier.x > -1.0f)
			{
				cameraModifier.x -= 0.1f;
			}
		}

		if (keys[GLFW_KEY_RIGHT])
		{
			_entFactory->GetPlayer().SetAngle(90.0f);
			if (cameraModifier.x < 1.0f)
			{
				cameraModifier.x += 0.1f;
			}
		}

		if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_LEFT])
		{
			_entFactory->GetPlayer().SetAngle(180.0f + 45.0f);

		}

		if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_LEFT])
		{
			_entFactory->GetPlayer().SetAngle(0.0f - 45.0f);
		}

		if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_RIGHT])
		{
			_entFactory->GetPlayer().SetAngle(180.0f - 45.0f);
		}

		if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_RIGHT])
		{
			_entFactory->GetPlayer().SetAngle(0.0f + 45.0f);
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
