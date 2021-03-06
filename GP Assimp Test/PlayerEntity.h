#pragma once
#include "Entity.h"
#include <string>
using namespace std;

class PlayerEntity : public Entity {
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


private:


	// set up the collider
	void SetUpCollider() {
		this->AddRigidBody(0);
	}
	// set up the sound(s)
	void SetUpAudio() {
		this->setSound("res/audio/explosion.wav");
	}
	// set up the model
	void SetUpModel() {
		//this->SetModel("res/models/Gladiator.obj");
		string path = "res/models/Gladiator.obj";
		const char *_tmpChar = path.c_str();
		this->model.LoadModel(_tmpChar);
	}
};