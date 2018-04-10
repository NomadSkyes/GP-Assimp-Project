#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager() {
};

void GameManager::SetUp(AudioSystem* _as, CollisionSystem* _cs){
	
	this->_AudioSystem = _as;
	this-> _CollisionSystem = _cs;
	// create the entity factory and pass in the references
	_entFactory = new EntityFactory();
	_entFactory->SetUp(_as, _cs);

	// add collision plane (placed here for convenience)
	btRigidBody* plane = this->_CollisionSystem->AddPlane();

	// set the first level to be 1
	this->level = 1;
};


void GameManager::GameBegin() {
	// create the entities
	// player
	SpawnPlayer();
	// drone
	SpawnEnemy(glm::vec3(-10.0f, 1.0f, 0.0f));
	
	
	// load the first level
	LoadLevel(2, glm::vec3(0,0,0));

	SpawnEnemy(glm::vec3(10.0f, 1.0f, 0.0f));

};

void GameManager::LoadNextLevel() {
	// load the next level 
	this->level++;
	cout << "current level: " << this->level << " " << endl;
	switch (this->level) {
	case 2:
		// level to the right of first level
		LoadLevel(3, glm::vec3(30.0f, 0.0f, 0.0f));
		break;
	case 3:
		// level to the right of second level
		LoadLevel(3, glm::vec3(45.0f, 0.0f, 0.0f));
		break;
	default:
		break;
	}
	
	
};

void GameManager::LoadLevel(int _type, glm::vec3 _pos){
	/*Entity* _ent =_entFactory->CreateEntity(_type, _pos);
	_ent->SetPlayable(true);
	this->Entities.push_back(_ent);*/
	this->Entities.push_back(_entFactory->CreateEntity(_type, _pos));

};

void GameManager::RemoveLevel(int _type) {
};

void GameManager::SpawnPlayer() {
	// spawns a player
	glm::vec3 _pos(6.0f, 1.0f, 0.0f);
	this->Entities.push_back(_entFactory->CreateEntity(0, _pos));
};

void GameManager::SpawnEnemy(glm::vec3 _pos) {
	// spawns a drone
	this->Entities.push_back(_entFactory->CreateEntity(1, _pos));
};

// return an entity by passing in an integer entity ID
Entity* GameManager::GetEntity(int _entityId){
	for (Entity* ent : this->Entities) {
		if (ent != nullptr) {
			if (ent->GetId() == _entityId) {
				return ent;
			}
		}
	}
	cerr << "no entity found :'( " << endl;
	return nullptr;
};
