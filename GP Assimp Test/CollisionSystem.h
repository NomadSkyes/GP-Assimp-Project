#pragma once

#include <btBulletDynamicsCommon.h>
#include <vector>
#include<iostream>
using namespace std;



class CollisionSystem {
private:
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;

	std::vector<btRigidBody*> bodies;

	void init() {
		this->collisionConfig = new btDefaultCollisionConfiguration();
		this->dispatcher = new btCollisionDispatcher(collisionConfig);
		this->broadphase = new btDbvtBroadphase();
		this->solver = new btSequentialImpulseConstraintSolver();

		this->world = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfig);
		this->world->setGravity(btVector3(0, -10, 0));	//set gravitational constant
	}



	// physics callback
	bool contactAddedCallbackBullet(btManifoldPoint& contactPoint, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2) {
		std::cout << "collision" << endl;
		return false;
	}

public:
	CollisionSystem() {
		init();
		
		// physics callback
		//gContactAddedCallback = contactAddedCallbackBullet;
	}
	// delete colliders
	~CollisionSystem() {
		for (int i = 0; i < bodies.size(); i++) {
			this->world->removeCollisionObject(bodies[i]);
			btMotionState* motionstate = bodies[i]->getMotionState();
			btCollisionShape* shape = bodies[i]->getCollisionShape();
			delete bodies[i];
			delete shape;
			delete motionstate;
		}
		delete this->dispatcher;
		delete this->collisionConfig;
		delete this->solver;
		delete this->world;
		delete this->broadphase;
	}

	// create a plane
	btRigidBody* AddPlane() {

		// create simple infinite plane
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0, 0, 0));
		btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
		btRigidBody* body = new btRigidBody(info);
		body->setRollingFriction(18.0f);
		AddBody(body);

		return body;
	}

	// create a sphere
	btRigidBody* AddSphere(float radius, float x, float y, float z, float mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(x, y, z));
		btSphereShape* sphere = new btSphereShape(radius);
		// check to see if object is static (mass = 0)
		btVector3 inertia(0, 0, 0);
		if (mass != 0.0f) {
			sphere->calculateLocalInertia(mass, inertia);
		}

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
		btRigidBody* body = new btRigidBody(info);
		AddBody(body);

		// testing limiting rotation of collider
		body->setAngularFactor(btVector3(0, 0, 0));

		//collision flags
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);


		return body;
	}

	// create a cylinder
	btRigidBody* AddCylinder(float diameter, float height, float x, float y, float z, float mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(x, y, z));
		btCylinderShape* cylinder = new btCylinderShape(btVector3(diameter / 2, height / 2, diameter / 2));
		// check to see if object is static (mass = 0)
		btVector3 inertia(0, 0, 0);
		if (mass != 0.0f) {
			cylinder->calculateLocalInertia(mass, inertia);
		}

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
		btRigidBody* body = new btRigidBody(info);
		AddBody(body);

		//collision flags
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		return body;
	}

	// create a cube
	btRigidBody* AddCube(double width, double height, double depth, float x, float y, float z, int mass) {
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(x, y, z));
		btBoxShape* box = new btBoxShape(btVector3(width / 2, height / 2, depth / 2));
		// check to see if object is static (mass = 0)
		btVector3 inertia(0, 0, 0);
		if (mass != 0.0f) {
			box->calculateLocalInertia(mass, inertia);
		}

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
		btRigidBody* body = new btRigidBody(info);
		AddBody(body);
		return body;
	}

	// step the simulation based on the time between frames
	// called every update
	void StepSimulation(float step) {
		this->world->stepSimulation(step);


		/*int numManifolds = world->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j < numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() < 0.f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;
				}
			}
		}*/

	}

	// add rigidbody to world
	void AddBody(btRigidBody* body) {
		this->world->addRigidBody(body);
		bodies.push_back(body);
	}

	void AddUserPointer(btRigidBody* _body, void *_user) {
		_body->setUserPointer(_user);
	}

	btDynamicsWorld* getWorld()
	{
		return this->world;
	}

	// draw ray using starting point, and end
	void ShootRaycast(const btVector3 &Start, btVector3 &End) {

		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

		// Perform raycast
		world->rayTest(Start, End, RayCallback);

		if (RayCallback.hasHit()) {


			//Entity* _entity = (Entity)(RayCallback.m_collisionObject().getUserPointer());
			//Entity* _entity = RayCallback.m_collisionObject.getUserPointer;
			//if (_entity != nullptr) {
		//		std::cout << "hit" << _entity->GetId << endl;
		//	}
			
			//const btCollisionObject* _object = RayCallback.m_collisionObject;
			//return _object->getUserPointer();
			// Do some clever stuff here
		}
	}

};

