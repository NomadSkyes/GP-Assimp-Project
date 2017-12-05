#pragma once

#include <btBulletDynamicsCommon.h>
#include <vector>

class CollisionSystem {
private:
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;

	vector<btRigidBody*> bodies;

	void init() {
		this->collisionConfig = new btDefaultCollisionConfiguration();
		this->dispatcher = new btCollisionDispatcher(collisionConfig);
		this->broadphase = new btDbvtBroadphase();
		this->solver = new btSequentialImpulseConstraintSolver();

		this->world = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfig);
		this->world->setGravity(btVector3(0, -10, 0));	//set gravitational constant


	}
public:
	CollisionSystem() {
		init();
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
		return body;
	}

	// create a cube
	btRigidBody* AddCube(float width, float height, float depth, float x, float y, float z, float mass) {
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
	void StepSimulation(float step) {
		this->world->stepSimulation(step);
	}

	// add rigidbody to world
	void AddBody(btRigidBody* body) {
		this->world->addRigidBody(body);
		bodies.push_back(body);
	}

};

