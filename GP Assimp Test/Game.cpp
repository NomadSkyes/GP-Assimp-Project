#include "stdafx.h"
#include "Game.h"


Game::Game() {
	
}

void Game::Init(GLFWwindow* _window) {
	

	// init irrklang
	audioSystem = new AudioSystem;

	// init bullet physics
	collisionSystem = new CollisionSystem();

	// create the EntityFactory
	_entFactory = new EntityFactory();
	_entFactory->SetUp(audioSystem, collisionSystem);

	// init window
	this->window = _window;
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	cameraModifier = glm::vec3(0.0f, 0.0f, 0.0f);
}

int Game::Play(){
	


	// set the listener's position to the camera's position
	audioSystem->UpdateListnerPosition(camera->GetPosition(), camera->GetFront());


	

	

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	

	if (nullptr == this->window)
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


	camera->SetAngle(-65.0f, -90.0f);

	// Draw in wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glm::mat4 projection = glm::perspective(camera->GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop

	std::cout << "just about to start" << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		//fixes camera on player
		playerCameraPos.x = _entFactory->GetPlayer()->GetPosition().x + cameraModifier.x;
		playerCameraPos.y = 10.0f;
		playerCameraPos.z = _entFactory->GetPlayer()->GetPosition().z + 5 + cameraModifier.z;
		camera->SetPosition(playerCameraPos);

		// set the listener's position to the camera's position
		audioSystem->UpdateListnerPosition(camera->GetPosition(), camera->GetFront());

		_entFactory->GetPlayer()->myTickCallback(collisionSystem->getWorld(), 1);

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

		glm::mat4 view = camera->GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model_a;
		glm::mat4 model_b;

		glm::mat4 enemy_1;

		glm::mat4 player_1;



		model_a = glm::translate(model_a, _entFactory->GetLevel_01()->GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_a = glm::scale(model_a, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
																	//model_a = glm::rotate(model_a, float(-90 * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_a));
		//lvl1_a.Draw(shader);
		_entFactory->GetLevel_01()->GetModel().Draw(shader);

		model_b = glm::translate(model_b, _entFactory->GetLevel_02()->GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_b = glm::scale(model_b, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_b));
		_entFactory->GetLevel_02()->GetModel().Draw(shader);


		enemy_1 = glm::translate(enemy_1, _entFactory->GetDrone()->GetPosition()); // Translate it down a bit so it's at the center of the scene
		enemy_1 = glm::scale(enemy_1, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		enemy_1 = glm::rotate(enemy_1, float(-90 * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		enemy_1 = glm::rotate(enemy_1, float(-_entFactory->GetDrone()->getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(enemy_1));
		_entFactory->GetDrone()->GetModel().Draw(shader);
		_entFactory->GetDrone()->LookAt(_entFactory->GetPlayer()->GetPosition());

		if (_entFactory->GetPlayer()->IsAlive()) {
			float test = _entFactory->GetPlayer()->getAngle();
			//cout << test << endl;
			player_1 = glm::translate(player_1, _entFactory->GetPlayer()->GetPosition()); // Translate it down a bit so it's at the center of the scene
			player_1 = glm::scale(player_1, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
			player_1 = glm::rotate(player_1, float(_entFactory->GetPlayer()->getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(player_1));
			//player.Draw(shader);
			_entFactory->GetPlayer()->GetModel().Draw(shader);
		}

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void Game::DoMovement() {
	if (_entFactory->GetPlayer()->IsAlive()) {
		// Camera controls
		if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_D])
		{
			_entFactory->GetPlayer()->Stop();
		}

		if (keys[GLFW_KEY_W])
		{
			_entFactory->GetPlayer()->ProcessKeyboard(ENTITY_UP, deltaTime);
		}


		if (keys[GLFW_KEY_S])
		{
			_entFactory->GetPlayer()->ProcessKeyboard(ENTITY_DOWN, deltaTime);
		}

		if (keys[GLFW_KEY_A])
		{
			_entFactory->GetPlayer()->ProcessKeyboard(ENTITY_LEFT, deltaTime);
		}

		if (keys[GLFW_KEY_D])
		{
			_entFactory->GetPlayer()->ProcessKeyboard(ENTITY_RIGHT, deltaTime);

		}

		if (keys[GLFW_KEY_SPACE])
		{
			// play sound
			_entFactory->GetPlayer()->playSound();
			_entFactory->GetPlayer()->Attack();
		}



		if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT])
		{
			if (keys[GLFW_KEY_UP])
			{
				_entFactory->GetPlayer()->SetAngle(180.0f);

				if (cameraModifier.z > -1.0f)
				{
					cameraModifier.z -= 0.1f;
				}
			}

			if (keys[GLFW_KEY_DOWN])
			{
				_entFactory->GetPlayer()->SetAngle(0.0f);
				if (cameraModifier.z < 1.0f)
				{
					cameraModifier.z += 0.1f;
				}
			}

			if (keys[GLFW_KEY_LEFT])
			{
				_entFactory->GetPlayer()->SetAngle(-90.0f);
				if (cameraModifier.x > -1.0f)
				{
					cameraModifier.x -= 0.1f;
				}
			}

			if (keys[GLFW_KEY_RIGHT])
			{
				_entFactory->GetPlayer()->SetAngle(90.0f);
				if (cameraModifier.x < 1.0f)
				{
					cameraModifier.x += 0.1f;
				}
			}

			if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_LEFT])
			{
				_entFactory->GetPlayer()->SetAngle(180.0f + 45.0f);

			}

			if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_LEFT])
			{
				_entFactory->GetPlayer()->SetAngle(0.0f - 45.0f);
			}

			if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_RIGHT])
			{
				_entFactory->GetPlayer()->SetAngle(180.0f - 45.0f);
			}

			if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_RIGHT])
			{
				_entFactory->GetPlayer()->SetAngle(0.0f + 45.0f);
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
	if (keys[GLFW_KEY_ENTER])
	{
		// kill player
		cout << "killing" << endl;
		_entFactory->GetPlayer()->Kill();
	}
}

void Game::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	Game *_game;
	_game = (Game *)glfwGetWindowUserPointer(window);

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			_game->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			_game->keys[key] = false;
		}
	}
}


void Game::MouseCallback(GLFWwindow *window, double xPos, double yPos) {
	Game *_game;
	_game = (Game *)glfwGetWindowUserPointer(window);

	if (_game->firstMouse)
	{
		_game->lastX = xPos;
		_game->lastY = yPos;
		_game->firstMouse = false;
	}

	GLfloat xOffset = xPos - _game->lastX;
	GLfloat yOffset = _game->lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	_game->lastX = xPos;
	_game->lastY = yPos;

	//camera->ProcessMouseMovement(xOffset, yOffset);
}