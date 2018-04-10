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

		//Left Barrier
		this->AddMultipleRigidBodies(0.5, 5, 50, this->position.x - 20, this->position.y, this->position.z, 0.0);
		//Right Barrier
		this->AddMultipleRigidBodies(0.5, 5, 50, this->position.x + 20, this->position.y, this->position.z, 0.0);
		//Top Barrier
		this->AddMultipleRigidBodies(50, 5, 0.5, this->position.x, this->position.y, this->position.z + 20, 0.0);
		//Bottom Barrier
		this->AddMultipleRigidBodies(50, 5, 0.5, this->position.x, this->position.y, this->position.z - 20, 0.0);


		// 2 large central blocks
		this->AddMultipleRigidBodies(4, 5, 4, this->position.x, this->position.y, this->position.z + 6, 0.0);
		this->AddMultipleRigidBodies(4, 5, 4, this->position.x, this->position.y, this->position.z - 6, 0.0);

		//Smaller side blocks
		this->AddMultipleRigidBodies(3.5, 5, 1, this->position.x + 7, this->position.y, this->position.z - 4.5, 0.0);
		this->AddMultipleRigidBodies(3.5, 5, 1, this->position.x + 7, this->position.y, this->position.z + 4.5, 0.0);
		this->AddMultipleRigidBodies(3.5, 5, 1, this->position.x - 7, this->position.y, this->position.z - 4.5, 0.0);
		this->AddMultipleRigidBodies(3.5, 5, 1, this->position.x - 7, this->position.y, this->position.z + 4.5, 0.0);
		//Outer miniblocks
		this->AddMultipleRigidBodies(1, 5, 1, this->position.x + 8.5, this->position.y, this->position.z - 8.5, 0.0);
		this->AddMultipleRigidBodies(1, 5, 1, this->position.x + 8.5, this->position.y, this->position.z + 8.5, 0.0);
		this->AddMultipleRigidBodies(1, 5, 1, this->position.x - 8.5, this->position.y, this->position.z - 8.5, 0.0);
		this->AddMultipleRigidBodies(1, 5, 1, this->position.x - 8.5, this->position.y, this->position.z + 8.5, 0.0);
		
	}

	// set up the model
	void SetUpModel() {
		//this->SetModel("res/models/ForestFloor.obj");
		string path = "res/models/ForestFloor.obj";
		const char *_tmpChar = path.c_str();
		this->model.LoadModel(_tmpChar);
	}
};