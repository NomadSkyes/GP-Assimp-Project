#include "stdafx.h"
#include "Game.h"


Game::Game() {
	
}

int Game::Init(GLFWwindow* _window) {
	

	// init irrklang
	audioSystem = new AudioSystem;

	// init bullet physics
	collisionSystem = new CollisionSystem();

	// create the game manager
	_gm = new GameManager();
	
	_gm->SetUp(audioSystem, collisionSystem);

	// init window
	this->window = _window;
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	cameraModifier = glm::vec3(0.0f, 0.0f, 0.0f);


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
	shader =  new Shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

	camera->SetAngle(-65.0f, -90.0f);

	// Draw in wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	projection = glm::perspective(camera->GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	return 0;
}

int Game::Play(){
	
	// start game
	_gm->GameBegin();

	// Game loop

	std::cout << "just about to start" << std::endl;
	double time = 0;
	while (!glfwWindowShouldClose(window))
	{

		double deltaTime = glfwGetTime() - time;
		Update(deltaTime);
		time = glfwGetTime();
		Render();

		// Check and call events
		glfwPollEvents();
		DoMovement();

		
	}

	glfwTerminate();
	return 0;
}

void Game::Update(double deltaTime) {
	//fixes camera on player
	playerCameraPos.x = _gm->GetEntity(0)->GetPosition().x + cameraModifier.x;
	playerCameraPos.y = 10.0f;
	playerCameraPos.z = _gm->GetEntity(0)->GetPosition().z + 5 + cameraModifier.z;
	camera->SetPosition(playerCameraPos);

	// set the listener's position to the camera's position
	audioSystem->UpdateListnerPosition(camera->GetPosition(), camera->GetFront());

	_gm->GetEntity(0)->myTickCallback(collisionSystem->getWorld(), 1);

	//collision
	collisionSystem->StepSimulation(deltaTime);	// step the physics simulation (default 1/60 seconds)

	_gm->GetEntity(1)->myTickCallback(collisionSystem->getWorld(), 1);
	_gm->GetEntity(3)->myTickCallback(collisionSystem->getWorld(), 1);
	_gm->GetEntity(1)->MoveToward(_gm->GetEntity(0)->GetPosition());
	_gm->GetEntity(3)->MoveToward(_gm->GetEntity(0)->GetPosition());



}
void Game::Render() {
	// Clear the colorbuffer
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();

	glm::mat4 view = camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Draw the loaded model
	glm::mat4 model_a;
	glm::mat4 model_b;
	glm::mat4 model_c;

	glm::mat4 enemy_1;
	glm::mat4 enemy_2;

	glm::mat4 player_1;


	// currently every entity must be known and access must be hard-codded to retrieve position & model

	if (_gm->levelNum() == 1)
	{
		model_a = glm::translate(model_a, _gm->GetEntity(2)->GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_a = glm::scale(model_a, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
																	//model_a = glm::rotate(model_a, float(-90 * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model_a));
		//lvl1_a.Draw(shader);
		_gm->GetEntity(2)->GetModel().Draw(shader);
	}
	

	//model_b = glm::translate(model_b, _gm->GetEntity(3)->GetPosition()); // Translate it down a bit so it's at the center of the scene
	//model_b = glm::scale(model_b, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_b));
	//_gm->GetEntity(3)->GetModel().Draw(shader);


	enemy_1 = glm::translate(enemy_1, _gm->GetEntity(1)->GetPosition()); // Translate it down a bit so it's at the center of the scene
	enemy_1 = glm::scale(enemy_1, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
	enemy_1 = glm::rotate(enemy_1, float(-180 * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	enemy_1 = glm::rotate(enemy_1, float(_gm->GetEntity(1)->getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(enemy_1));
	_gm->GetEntity(1)->GetModel().Draw(shader);
	_gm->GetEntity(1)->LookAt(_gm->GetEntity(0)->GetPosition());

	enemy_2 = glm::translate(enemy_2, _gm->GetEntity(3)->GetPosition()); // Translate it down a bit so it's at the center of the scene
	enemy_2 = glm::scale(enemy_2, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
	enemy_2 = glm::rotate(enemy_2, float(-180 * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	enemy_2 = glm::rotate(enemy_2, float(_gm->GetEntity(3)->getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(enemy_2));
	_gm->GetEntity(3)->GetModel().Draw(shader);
	_gm->GetEntity(3)->LookAt(_gm->GetEntity(0)->GetPosition());

	if (_gm->levelNum() >= 2)
	{
		_gm->GetEntity(2)->Kill();
		model_b = glm::translate(model_b, _gm->GetEntity(4)->GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_b = glm::scale(model_b, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		//model_a = glm::rotate(model_a, float(-90 * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model_b));
		//lvl1_a.Draw(shader);
		_gm->GetEntity(4)->GetModel().Draw(shader);
	}

	if (_gm->levelNum() >= 3)
	{
		model_c = glm::translate(model_c, _gm->GetEntity(5)->GetPosition()); // Translate it down a bit so it's at the center of the scene
		model_c = glm::scale(model_c, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
																	//model_a = glm::rotate(model_a, float(-90 * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model_c));
		//lvl1_a.Draw(shader);
		_gm->GetEntity(5)->GetModel().Draw(shader);
	}

	if (_gm->GetEntity(0)->IsAlive()) {
		float test = _gm->GetEntity(0)->getAngle();
		//cout << test << endl;
		player_1 = glm::translate(player_1, _gm->GetEntity(0)->GetPosition()); // Translate it down a bit so it's at the center of the scene
		player_1 = glm::scale(player_1, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
		player_1 = glm::rotate(player_1, float(_gm->GetEntity(0)->getAngle() * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(player_1));
		//player.Draw(shader);
		_gm->GetEntity(0)->GetModel().Draw(shader);
	}

	// Swap the buffers
	glfwSwapBuffers(window);
}

void Game::DoMovement() {
	if (_gm->GetEntity(0)->IsAlive()) {
		// Camera controls
		if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_D])
		{
			_gm->GetEntity(0)->Stop();
		}

		if (keys[GLFW_KEY_W])
		{
			_gm->GetEntity(0)->ProcessKeyboard(ENTITY_UP, deltaTime);
		}


		if (keys[GLFW_KEY_S])
		{
			_gm->GetEntity(0)->ProcessKeyboard(ENTITY_DOWN, deltaTime);
		}

		if (keys[GLFW_KEY_A])
		{
			_gm->GetEntity(0)->ProcessKeyboard(ENTITY_LEFT, deltaTime);
		}

		if (keys[GLFW_KEY_D])
		{
			_gm->GetEntity(0)->ProcessKeyboard(ENTITY_RIGHT, deltaTime);

		}
		if (keys[GLFW_KEY_SPACE])
		{
			// play sound
			_gm->GetEntity(0)->Attack();
		}

		if (keys[GLFW_KEY_TAB])
		{

			// load next level for testing
			_gm->LoadNextLevel();
		}

		if (keys[GLFW_KEY_ENTER])
		{
			// kill player
			cout << "killing" << endl;
			_gm->GetEntity(0)->Kill();
		}
		

		if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT])
		{
			if (keys[GLFW_KEY_UP])
			{
				_gm->GetEntity(0)->SetAngle(180.0f);

				if (cameraModifier.z > -1.0f)
				{
					cameraModifier.z -= 0.1f;
				}
			}

			if (keys[GLFW_KEY_DOWN])
			{
				_gm->GetEntity(0)->SetAngle(0.0f);
				if (cameraModifier.z < 1.0f)
				{
					cameraModifier.z += 0.1f;
				}
			}

			if (keys[GLFW_KEY_LEFT])
			{
				_gm->GetEntity(0)->SetAngle(-90.0f);
				if (cameraModifier.x > -1.0f)
				{
					cameraModifier.x -= 0.1f;
				}
			}

			if (keys[GLFW_KEY_RIGHT])
			{
				_gm->GetEntity(0)->SetAngle(90.0f);
				if (cameraModifier.x < 1.0f)
				{
					cameraModifier.x += 0.1f;
				}
			}

			if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_LEFT])
			{
				_gm->GetEntity(0)->SetAngle(180.0f + 45.0f);

			}

			if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_LEFT])
			{
				_gm->GetEntity(0)->SetAngle(0.0f - 45.0f);
			}

			if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_RIGHT])
			{
				_gm->GetEntity(0)->SetAngle(180.0f - 45.0f);
			}

			if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_RIGHT])
			{
				_gm->GetEntity(0)->SetAngle(0.0f + 45.0f);
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
