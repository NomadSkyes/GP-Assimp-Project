#pragma once
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "Entity.h"
#include "EntityFactory.h"


class GameManager {
public:
	// constructor
	GameManager();

	// store references to the audio and physics classes
	void SetUp(AudioSystem *_as, CollisionSystem *_cs);

	// spawn objects and start gameplay
	void GameBegin();

	// load the next level
	void LoadNextLevel();

	// load in a level
	void LoadLevel(int _type, glm::vec3 _pos);

	//remove a loaded level
	void RemoveLevel(int _type);

	// spawn a player
	void SpawnPlayer();

	// spawn an enemy
	void SpawnEnemy(glm::vec3 _pos);

	// return an entity
	Entity* GetEntity(int _entityId);



private:
	// keep track of the current level
	int level;
	// store all entities
	vector<Entity*> Entities;

	// create the EntityFactory
	EntityFactory* _entFactory = nullptr;

	// reference to audio system
	AudioSystem* _AudioSystem = nullptr;

	// reference to collision system
	CollisionSystem* _CollisionSystem = nullptr;

	
};
