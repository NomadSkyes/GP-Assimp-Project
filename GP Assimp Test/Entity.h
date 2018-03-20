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

// Audio
#include "AudioSystem.h"

#define PI 3.14159265

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

	// Constructor with scalar values
	Entity(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(ENTITY_SPEED)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
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
		if (_cs != nullptr) {
			btTransform trans;
			_rb->getMotionState()->getWorldTransform(trans);
			glm::vec3 updatedPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			this->position = updatedPos;
			return this->position;
		}
		else {
			cout << "no CollisionSystem reference! (1)" << endl;
		}
	}

	glm::vec3 GetFront()
	{
		return this->front;
	}

	void Stop()
	{
		if (_cs != nullptr) {
			_rb->clearForces();
			_rb->setLinearVelocity(btVector3(0, 0, 0));
			_rb->setAngularVelocity(btVector3(0, 0, 0));
		}
		else {
			cout << "no CollisionSystem reference! (2)" << endl;
		}
	}

	void myTickCallback(btDynamicsWorld *world, btScalar timeStep) {
		if (_cs != nullptr) {
			// mShipBody is the spaceship's btRigidBody
			btVector3 velocity = _rb->getLinearVelocity();
			btScalar speed = velocity.length();
			if (speed > mMaxSpeed) {
				velocity *= mMaxSpeed / speed;
				if (_rb != nullptr) {
					_rb->setLinearVelocity(velocity);
				}
			}
		}
		else {
			cout << "no CollisionSystem reference! (3)" << endl;
		}
	}


	void Attack()
	{
		// shoot raycast
		btVector3 pos(this->position.x, this->position.y, this->position.z);
		btVector3 dest(this->front.x, this->front.y, this->front.z);

		if (_rb != nullptr || _cs != nullptr) {
			//_cs->ShootRaycast(pos, (dest * 2));
			_cs->ShootRaycast(pos, dest);
		}
		else {
			cout << "no CollisionSystem reference! (4)" << endl;
		}

	}

	void SetPosition(glm::vec3 newPos)
	{
		if (_cs != nullptr) {
			if (_rb != nullptr) {
				_rb->activate();

				btVector3 temp(newPos.x, newPos.y, newPos.z);
				//_rb->setLinearVelocity(temp);
				_rb->applyImpulse((temp / 5), (btVector3(this->front.x, this->front.y, this->front.z) / 5));

				btTransform trans;
				_rb->getMotionState()->getWorldTransform(trans);
				glm::vec3 updatedPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
				this->position = updatedPos;
			}
			else {
				for (int i = 0; i < _multiRb.size(); i++) {
					_multiRb[i]->activate();

					btVector3 temp(newPos.x, newPos.y, newPos.z);
					_multiRb[i]->applyImpulse((temp / 5), (btVector3(this->front.x, this->front.y, this->front.z) / 5));

					btTransform trans;
					_multiRb[i]->getMotionState()->getWorldTransform(trans);
					glm::vec3 updatedPos(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
					this->position = updatedPos;
				}
			}
		}
		else {
			cout << "no CollisionSystem reference! (5)" << endl;
		}
	}

	void AddRigidBody(int type) {
		if (_cs != nullptr) {
			switch (type) {
				// sphere
			case 0:
				_rb = _cs->AddSphere(1.0, this->position.x, this->position.y, this->position.z, 1);
				break;
				// cylinder
			case 1:
				_rb = _cs->AddCylinder(1.0, 2.0, this->position.x, this->position.y, this->position.z, 1);
				break;
				// cube
			case 2:
				_rb = _cs->AddCube(1.0, 1.0, 1.0, this->position.x, this->position.y, this->position.z, 1);
				break;
			default:
				break;
			}
		}
		else {
			cout << "no CollisionSystem reference! (6)" << endl;
		}
	}

	// add multiple rigidbodies to an object
	void AddMultipleRigidBodies(float dx, float dy, float dz, float posx, float posy, float posz, float mass) {
		if (_cs != nullptr) {
			btRigidBody* _mRb = _cs->AddCube(dx, dz, dz, posx, posy, posz, mass);
			_multiRb.push_back(_mRb);
		}
		else {
			cout << "no CollisionSystem reference! (7)" << endl;
		}
	}


	// pass through a pointer to the collision system
	void SetCollisionSystem(CollisionSystem* cs) {
		this->_cs = cs;
	}

	float getAngle()
	{
		return this->pitch;
	}

	void SetAngle(float newPitch)
	{
		this->pitch = newPitch;
	}

	void setSpeed(double newMaxSpeed)
	{
		mMaxSpeed = newMaxSpeed;
	}

	// audio
	// pass through a pointer to the audio system
	void SetAudioSystem(AudioSystem* as) {
		this->_as = as;
	}
	// add sound source
	void setSound(string path) {
		if (_as != nullptr) {
			testSound = _as->AddSoundSource(path, 1);
		}
		else {
			cout << "no AudioSystem reference! (1)" << endl;
		}
	}

	// play the sound at the entity's position
	void playSound() {
		if (_as != nullptr) {
			_as->PlaySound3D(testSound, position);
		}
		else {
			cout << "no AudioSystem reference! (2)" << endl;
		}
	}

	//get angle to point
	void LookAt(glm::vec3 target)
	{
		float dot = (this->position.x*target.x) + (this->position.z*target.z);
		float det = (this->position.x*target.z) + (this->position.z*target.x);
		SetAngle(atan2(det, dot) * (180.0 / PI));
		cout << atan2(det, dot) * (180.0 / PI) << endl;

		//float dot = (this->position.x*target.x) + (this->position.y*target.y) + (this->position.z*target.z);
		//float lenSq1 = (this->position.x*this->position.x) + (this->position.y*this->position.y) + (this->position.z*this->position.z);
		//float lenSq2 = (target.x*target.x) + (target.y*target.y) + (target.z*target.z);
		//float output = acos(dot / sqrt(lenSq1 * lenSq2));
		//SetAngle(output);
		
	}

	// model
	void SetModel(string path) {
		if (!path.empty()) {
			const char *_tmpChar = path.c_str();
			model.LoadModel(_tmpChar);
		}
	}

	// return entity model
	Model GetModel() {
		return this->model;
	}


private:
	// Entity Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// model;
	Model model;

	btScalar mMaxSpeed = 4.0;

	// colliders
	CollisionSystem* _cs = nullptr;
	btRigidBody* _rb; 
	vector<btRigidBody*> _multiRb;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Entity options
	GLfloat movementSpeed = 10;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// entity sounds
	AudioSystem* _as = nullptr;
	ISoundSource* testSound;

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
