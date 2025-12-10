#pragma once
#include <PxRigidBody.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include "RenderUtils.hpp"
#include "Vector3.h"
class PhysicsObject{
public:
	PhysicsObject(physx::PxPhysics* gPhysics):physics(gPhysics) {
		mat = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	};
protected:
	physx::PxPhysics* physics = nullptr;
	physx::PxRigidActor* actor = nullptr;
	physx::PxMaterial* mat = nullptr;
};
class StaticPhysicsObject : public PhysicsObject {
public:
	StaticPhysicsObject(physx::PxPhysics* gPhysics,custom::Vector3 v = custom::Vector3::blank()) :PhysicsObject(gPhysics) {
		actor = physics->createRigidStatic(physx::PxTransform(v.converted()));
	}
};
class DynamicPhysicsObject : public PhysicsObject {
public:
	DynamicPhysicsObject(physx::PxPhysics* gPhysics,custom::Vector3 v = custom::Vector3::blank()) :PhysicsObject(gPhysics) {
		actor = physics->createRigidDynamic(physx::PxTransform(v.converted()));
	}
};
class SphereParticle : public DynamicPhysicsObject {
public:
	SphereParticle(physx::PxPhysics* gPhysics, int size, custom::Vector3 v = custom::Vector3::blank()):
		DynamicPhysicsObject(gPhysics,v) {
		auto sphere = gPhysics->createShape(physx::PxSphereGeometry(size),*mat);
		actor->attachShape(*sphere);
	}
};