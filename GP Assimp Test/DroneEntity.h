#pragma once
#include "Entity.h"
#include <string>
using namespace std;

class DroneEntity : public Entity {
public:


	// set up the pre defined variables
	void SetUp() {
		this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->yaw = -90.0f;
		this->pitch = 0.0f;
		this->updateEntityVectors();
		this->front = glm::vec3(0.0f, 0.0f, -1.0f);
		this->movementSpeed = 4.0f;
		SetUpCollider();
		SetUpAudio();
		SetUpModel();
	}

	

	void SetAngle(float newPitch)
	{
		this->pitch = newPitch;
	}

	

private:

	// set up the collider
	void SetUpCollider() {
		this->AddRigidBody(1);
	}
	// set up the sound(s)
	void SetUpAudio() {
		// bug when two entities share same sound  file
		this->setSound("res/audio/explosion02.wav");
	}
	// set up the model
	void SetUpModel() {
		//this->SetModel("res/models/Drone.obj");
		string path = "res/models/Drone.obj";
		const char *_tmpChar = path.c_str();
		this->model.LoadModel(_tmpChar);
	}
};