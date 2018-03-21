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
		SetStartPosition();
		SetUpCollider();
		SetUpAudio();
		SetUpModel();
	}

	//get angle to point
	void LookAt(glm::vec3 target)
	{
		float dot = (this->position.x*target.x) + (this->position.z*target.z);
		float det = (this->position.x*target.z) + (this->position.z*target.x);
		SetAngle(atan2(det, dot) * (180.0 / PI));
		//cout << atan2(det, dot) * (180.0 / PI) << endl;

		//float dot = (this->position.x*target.x) + (this->position.y*target.y) + (this->position.z*target.z);
		//float lenSq1 = (this->position.x*this->position.x) + (this->position.y*this->position.y) + (this->position.z*this->position.z);
		//float lenSq2 = (target.x*target.x) + (target.y*target.y) + (target.z*target.z);
		//float output = acos(dot / sqrt(lenSq1 * lenSq2));
		//SetAngle(output);

	}

	void SetAngle(float newPitch)
	{
		this->pitch = newPitch;
	}

	float getAngle()
	{
		return this->pitch;
	}

private:
	// Set starting position
	void SetStartPosition() {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	}
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