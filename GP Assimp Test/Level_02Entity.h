#pragma once
#include "Entity.h"
#include <string>
using namespace std;

class Level_02Entity : public Entity {
public:


	// set up the pre defined variables
	void SetUp() {
		this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->yaw = -90.0f;
		this->pitch = 0.0f;
		this->updateEntityVectors();
		this->front = glm::vec3(0.0f, 0.0f, -1.0f);
		this->movementSpeed = 4.0f;
		SetStartPosition();
		SetUpCollider();
		SetUpModel();
	}



private:
	// Set starting position
	void SetStartPosition() {
		this->position = glm::vec3(0.0f, -0.5f, 0.0f);
	}
	// set up the collider
	void SetUpCollider() {

		this->AddMultipleRigidBodies(0.5, 5, 15, -7.5, 0, 0, 0.0);
		this->AddMultipleRigidBodies(0.5, 5, 15, 7.5, 0, 0, 0.0);
		this->AddMultipleRigidBodies(15, 5, 0.5, 0, 0, -7.5, 0.0);
		this->AddMultipleRigidBodies(15, 5, 0.5, 0, 0, 7.5, 0.0);
		this->AddMultipleRigidBodies(2.5, 5, 2.5, 0, 0, 0, 0.0);
	}

	// set up the model
	void SetUpModel() {
		//this->SetModel("res/models/ForestFloor.obj");
		string path = "res/models/debugLVL.obj";
		const char *_tmpChar = path.c_str();
		this->model.LoadModel(_tmpChar);
	}
};