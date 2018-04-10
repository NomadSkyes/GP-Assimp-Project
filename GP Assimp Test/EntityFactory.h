#pragma once
//#include "stdafx.h"

//entities
#include "Entity.h"
#include "PlayerEntity.h"
#include "DroneEntity.h"
#include "Level_01Entity.h"
#include "Level_02Entity.h"

#include <vector>



class EntityFactory {
public:
	EntityFactory() {

	}

	void SetUp(AudioSystem * _as, CollisionSystem * _cs){

	this->_AudioSystem = _as;
	this->_CollisionSystem = _cs;
	}
	

	// create an entity:
	/*
	Entites are currently assigned using ints starting from 0, 
	done for convenience. i.e. 0 for player, 1 for drone, 
	2 for test level, 3 for forrest level. etc.
	*/
	Entity* CreateEntity(int _type, glm::vec3 _pos) {
		switch (_type) {
			case 0:
				return CreatePlayerEntity(_pos);
				break;
			case 1:
				return CreateDroneEntity(_pos);
				break;
			case 2:
				return CreateLevel_01Entity(_pos);
				break;
			case 3:
				return CreateLevel_02Entity(_pos);
				break;
			default:
				return nullptr;
				break;
		}
	}

	

private:
	// keep track of all entities
	int EntityIdCounter = 0;
	
	// reference to audio system
	AudioSystem* _AudioSystem = nullptr;

	// reference to collision system
	CollisionSystem* _CollisionSystem = nullptr;

	Entity* CreatePlayerEntity( glm::vec3 _pos) {
		PlayerEntity* playerEntity = new PlayerEntity();
		playerEntity->SetCollisionSystem(this->_CollisionSystem);
		playerEntity->SetAudioSystem(this->_AudioSystem);
		playerEntity->SetStartPosition(_pos);
		playerEntity->SetUp();
		playerEntity->SetId(EntityIdCounter);
		
		EntityIdCounter++;
		return playerEntity;
	}
	Entity* CreateDroneEntity( glm::vec3 _pos) {
		DroneEntity* droneEntity = new DroneEntity();
		droneEntity->SetCollisionSystem(this->_CollisionSystem);
		droneEntity->SetAudioSystem(this->_AudioSystem);
		droneEntity->SetStartPosition(_pos);
		droneEntity->SetUp();
		droneEntity->SetId(EntityIdCounter);
		
		EntityIdCounter++;
		return droneEntity;
	}
	Entity* CreateLevel_01Entity( glm::vec3 _pos) {
		Level_01Entity* Level_01 = new Level_01Entity();
		Level_01->SetCollisionSystem(this->_CollisionSystem);
		Level_01->SetStartPosition(_pos);
		Level_01->SetUp();
		Level_01->SetId(EntityIdCounter);
		
		EntityIdCounter++;
		return Level_01;
	}
	Entity* CreateLevel_02Entity( glm::vec3 _pos) {
		Level_02Entity* Level_02 = new Level_02Entity();
		Level_02->SetCollisionSystem(this->_CollisionSystem);
		Level_02->SetStartPosition(_pos);
		Level_02->SetUp();
		Level_02->SetId(EntityIdCounter);
		
		EntityIdCounter++;
		return Level_02;
	}
};

