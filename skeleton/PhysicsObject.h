#pragma once
#include <PxRigidBody.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include "RenderUtils.hpp"
#include "Vector3.h"
#include <list>
#define PI 3.1416
namespace physics {
	struct phys_particle_config {
		custom::Vector3 position;
		custom::Vector3 velocity;
		double mass;
		double size;
		Vector4 color;
		double lifetime;
		phys_particle_config(custom::Vector3 p = custom::Vector3::blank(),
			custom::Vector3 v = custom::Vector3::blank(),
			double m = 1.0,
			double s = 10,
			Vector4 c = { 1,1,1,1 },
			double l = 1.0) {
			position = p;
			velocity = v;
			mass = m;
			size = s;
			color = c;
			lifetime = l;
		}
		void operator=(phys_particle_config& other) {
			position = other.position;
			velocity = other.velocity;
			lifetime = other.lifetime;
			mass = other.mass;
			color = other.color;
		}
	};
	class PhysicsObject {
	public:
		PhysicsObject(physx::PxScene* s) :scene(s) {
			auto gphysics = &PxGetPhysics();
			mat = gphysics->createMaterial(1.0f,1.0f,1.0f);
		};
		~PhysicsObject() {
			if (_renderItem)
				DeregisterRenderItem(_renderItem);
			for (auto c : children) if (c) delete c;
		}
		virtual void step(double dt);
		void setParent(PhysicsObject* parent);
		void addChild(PhysicsObject* child);
		inline bool isAlive() const { return alive; }
		custom::Vector3 getPosition();
	protected:
		bool alive = true;
		bool timed = false;
		double lifetime = -1.0;
		physx::PxScene* scene = nullptr;
		physx::PxRigidActor* actor = nullptr;
		physx::PxMaterial* mat = nullptr;
		RenderItem* _renderItem = nullptr;
		std::list<PhysicsObject*> children = std::list<PhysicsObject*>();
		PhysicsObject* parent = nullptr;

	};
	class StaticPhysicsObject : public PhysicsObject {
	public:
		StaticPhysicsObject(physx::PxScene* s, phys_particle_config config = phys_particle_config()) :PhysicsObject(s) {
			auto gphysics = &PxGetPhysics();
			stcActor = gphysics->createRigidStatic(physx::PxTransform(config.position.converted(), physx::PxQuat(physx::PxIdentity)));
			stcActor->getGlobalPose();
			actor = stcActor;
			scene->addActor(*stcActor);
		}
	protected:
		physx::PxRigidStatic* stcActor = nullptr;
	};
	class DynamicPhysicsObject : public PhysicsObject {
	public:
		DynamicPhysicsObject(physx::PxScene* s, phys_particle_config config = phys_particle_config()) :PhysicsObject(s) {
			auto gphysics = &PxGetPhysics();
			dynActor = gphysics->createRigidDynamic(physx::PxTransform(config.position.converted(), physx::PxQuat(physx::PxIdentity)));
			actor = dynActor;
			scene->addActor(*dynActor);
		}
		custom::Vector3 getDirection();
		void addForce(custom::Vector3 f);
	protected:
		physx::PxRigidDynamic* dynActor = nullptr;

	};
	class SphereParticle : public DynamicPhysicsObject {
	public:
		SphereParticle(physx::PxScene* s, phys_particle_config config = phys_particle_config()) :
			DynamicPhysicsObject(s, config) {
			auto gphysics = &PxGetPhysics();
			auto sphere = gphysics->createShape(physx::PxSphereGeometry(config.size), *mat);
			_renderItem = new RenderItem(sphere,dynActor, {1,1,1,1});
			dynActor->attachShape(*sphere);
			dynActor->setMass(config.mass);
			physx::PxRigidBodyExt::updateMassAndInertia(*dynActor, config.mass / (4 * pow(config.size, 3) * PI / 3));
			dynActor->setLinearVelocity(config.velocity.converted());
		}
	protected:
	};
}