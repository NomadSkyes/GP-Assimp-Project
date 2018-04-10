#include "stdafx.h"
#include "Game.h"
#include <GLFW/glfw3.h>

int main(int argc, const char* argv[]) {

	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(800, 600, "Super Battle Arena!", nullptr, nullptr);

	// create the game
	Game game; 

	// for glfw input
	// from https://gamedev.stackexchange.com/questions/71721/how-can-i-forward-glfws-keyboard-input-to-another-object
	glfwSetWindowUserPointer(window, &game);

	// initialise game 
	game.Init(window);

	// play game
	game.Play();

	std::cout << "Game over" << std::endl;

	return 0;
}
