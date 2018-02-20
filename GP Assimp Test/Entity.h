#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//bullet
#include "CollisionSystem.h"

// models
#include "Model.h"

// Defines several possible options for Entity movement. Used as abstraction to stay away from window-system specific input methods
enum Entity_Movement
{
	ENTITY_UP,
	ENTITY_DOWN,
	ENTITY_LEFT,
	ENTITY_RIGHT
};

// Default Entity values
const GLfloat ENTITY_YAW = -90.0f;
const GLfloat ENTITY_PITCH = 0.0f;
const GLfloat ENTITY_SPEED = 4.0f;



// An abstract Entity class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Entity
{
public:
	// Constructor with vectors
	Entity(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = ENTITY_YAW, GLfloat pitch = ENTITY_PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(ENTITY_SPEED)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateEntityVectors();

		
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of Entity defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Entity_Movement direction, GLfloat deltaTime)
	{
		//GLfloat velocity = this->movementSpeed * deltaTime;
		GLfloat velocity = this->movementSpeed;

		if (direction == ENTITY_UP)
		{
			//this->position += this->front * velocity;
		
			glm::vec3 movementVector = this->front * velocity;
			SetPosition(movementVector);
		}

		if (direction == ENTITY_DOWN)
		{
			//this->position -= this->front * velocity;
			glm::vec3 movementVector = -this->front * velocity;
			SetPosition(movementVector);

		}

		if (direction == ENTITY_LEFT)
		{
			//this->position -= this->right * velocity;
			glm::vec3 movementVector = -this->right * velocity;
			SetPosition(movementVector);

		}

		if (direction == ENTITY_RIGHT)
		{
			//this->position += this->right * velocity;
			glm::vec3 movementVector = this->right * velocity;
			SetPosition(movementVector);

		}
	}


	glm::vec3 GetPosition()
	{
		btTransform trans;
		_rb->getMotionState()->getWorldTransform(trans);
		glm::vec3 updatedPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		this->position = updatedPos;
		return this->position;
	}

	glm::vec3 GetFront()
	{
		return this->front;
	}

	void SetPosition(glm::vec3 newPos)
	{
		_rb->activate();

		btVector3 temp(newPos.x, newPos.y, newPos.z);
		_rb->applyForce(temp, btVector3(this->front.x, this->front.y, this->front.z));

		btTransform trans;
		_rb->getMotionState()->getWorldTransform(trans);
		glm::vec3 updatedPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		this->position = updatedPos;
	}

	void AddRigidBody(CollisionSystem* _cs) {
		// adding a sphere collider to test with player
		_rb = _cs->AddSphere(1.0, this->position.x, this->position.y, this->position.z, 1);

	}

	float getAngle()
	{
		return this->pitch;
	}

	void SetAngle(float newPitch)
	{
		this->pitch = newPitch;
	}


private:
	// Entity Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;


	// collider
	btRigidBody* _rb; 

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Entity options
	GLfloat movementSpeed = 10;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// Calculates the front vector from the Entity's (updated) Eular Angles
	void updateEntityVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};
