#pragma once
//#include "stdafx.h"

//entities
#include "Entity.h"
#include "PlayerEntity.h"
#include "DroneEntity.h"
#include "Level_01Entity.h"
#include "Level_02Entity.h"

#include <vector>

// define entity types
//enum EntityType {
//	PLAYER,
//	DRONE,
//	LEVEL_01,
//	TEST_LEVEL
//};

class EntityFactory {
public:
	EntityFactory() {

	}

	void SetUp(AudioSystem* _as, CollisionSystem* _cs) {
		this->_AudioSystem = _as;
		this->_CollisionSystem = _cs;

		// add collision plane (placed here for convenience)
		btRigidBody* plane = this->_CollisionSystem->AddPlane();
	}

	// create an entity:
	// PLAYER
	// DRONE
	// LEVEL_01
	void CreateEntity(int type) {
		//switch (type) {
		//case PLAYER:
		//	playerEntity.SetCollisionSystem(this->_CollisionSystem);
		//	playerEntity.SetAudioSystem(this->_AudioSystem);
		//	//Entities.push_back(new Entity(playerEntity));
		//	break;
		//case DRONE:
		//	droneEntity.SetCollisionSystem(this->_CollisionSystem);
		//	droneEntity.SetAudioSystem(this->_AudioSystem);
		//	//Entities.push_back(new Entity(droneEntity));
		//	break;
		//case LEVEL_01:
		//	Level_01.SetCollisionSystem(this->_CollisionSystem);
		//	//Entities.push_back(new Entity(Level_01));
		//	break;
		//case TEST_LEVEL:
		//	break;
		//default:
		//	break;
		//}
		switch (type) {
		case 0:
			playerEntity.SetCollisionSystem(this->_CollisionSystem);
			playerEntity.SetAudioSystem(this->_AudioSystem);
			playerEntity.SetUp();
			//Entities.push_back(new Entity(playerEntity));
			break;
		case 1:
			droneEntity.SetCollisionSystem(this->_CollisionSystem);
			droneEntity.SetAudioSystem(this->_AudioSystem);
			droneEntity.SetUp();

			//Entities.push_back(new Entity(droneEntity));
			break;
		case 2:
			Level_01.SetCollisionSystem(this->_CollisionSystem);
			Level_01.SetUp();

			//Entities.push_back(new Entity(Level_01));
			break;
		case 3:
			Level_02.SetCollisionSystem(this->_CollisionSystem);
			Level_02.SetUp();
			break;
		default:
			break;
		}
	}

	// return the player entity
	PlayerEntity GetPlayer() {
		return this->playerEntity;
	}
	// return the Drone entity
	DroneEntity GetDrone() {
		return this->droneEntity;
	}
	// return the Level_01 entity
	Level_01Entity GetLevel_01() {
		return this->Level_01;
	}

	// return the Level_02 entity
	Level_02Entity GetLevel_02() {
		return this->Level_02;
	}

private:
	// list of all entities
	vector<Entity> Entities;

	//for testing
	PlayerEntity playerEntity;
	DroneEntity droneEntity;
	Level_01Entity Level_01;
	Level_02Entity Level_02;

	// reference to audio system
	AudioSystem* _AudioSystem = nullptr;

	// reference to collision system
	CollisionSystem* _CollisionSystem = nullptr;


};

