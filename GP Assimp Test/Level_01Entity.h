#pragma once
#include "Entity.h"
#include <string>
using namespace std;

class Level_01Entity : public Entity {
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
		SetUpModel();
	}

	

private:

	// set up the colliders
	void SetUpCollider() {

		//left top
		this->AddMultipleRigidBodies(0.5,4.5, 5, this->position.x - 7.5, this->position.y, this->position.z + 4, 0.0);
		//left bottom
		this->AddMultipleRigidBodies(0.5,4.5, 5, this->position.x - 7.5, this->position.y, this->position.z  - 4, 0.0);

		//right top
		this->AddMultipleRigidBodies(0.5, 4.5, 5, this->position.x + 7.5, this->position.y, this->position.z + 4, 0.0);
		//right bottom
		this->AddMultipleRigidBodies(0.5, 4.5, 5, this->position.x + 7.5, this->position.y, this->position.z - 4, 0.0);


		// top left
		this->AddMultipleRigidBodies(5, 4.4, 0.5, this->position.x - 4, this->position.y, this->position.z - 7.5, 0.0);
		// top right
		this->AddMultipleRigidBodies(5, 4.4, 0.5, this->position.x + 4, this->position.y, this->position.z - 7.5, 0.0);

		// bottom left
		this->AddMultipleRigidBodies(5, 4.4, 0.5, this->position.x - 4, this->position.y, this->position.z + 7.5, 0.0);
		// bottom right
		this->AddMultipleRigidBodies(5, 4.4, 0.5, this->position.x + 4, this->position.y, this->position.z + 7.5, 0.0);

		// center
		this->AddMultipleRigidBodies(2.5, 5, 2.5, this->position.x, this->position.y, this->position.z, 0.0);
	}

	// set up the model
	void SetUpModel() {
		//this->SetModel("res/models/ForestFloor.obj");
		string path = "res/models/debugLVL.obj";
		const char *_tmpChar = path.c_str();
		this->model.LoadModel(_tmpChar);
	}
};