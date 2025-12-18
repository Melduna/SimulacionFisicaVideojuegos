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
		bool timed;
		bool gravity;
		phys_particle_config(custom::Vector3 p = custom::Vector3::blank(),
			custom::Vector3 v = custom::Vector3::blank(),
			double m = 1.0,
			double s = 10,
			Vector4 c = { 1,1,1,1 },
			double l = 1.0,
			bool t = false,
			bool g = false) {
			position = p;
			velocity = v;
			mass = m;
			size = s;
			color = c;
			lifetime = l;
			timed = t;
			gravity = g;
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
		PhysicsObject(physx::PxScene* s);
		~PhysicsObject();
		virtual void step(double dt);
		virtual void translate(custom::Vector3 v);
		void setParent(PhysicsObject* parent);
		void addChild(PhysicsObject* child);
		void setOpacity(double a);
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

		custom::Vector3 last_pos;
		custom::Vector3 new_pos;
		custom::Vector3 delta;

	};
	class StaticPhysicsObject : public PhysicsObject {
	public:
		StaticPhysicsObject(physx::PxScene* s, phys_particle_config config = phys_particle_config());
	protected:
		physx::PxRigidStatic* stcActor = nullptr;
	};
	class DynamicPhysicsObject : public PhysicsObject {
	public:
		DynamicPhysicsObject(physx::PxScene* s, phys_particle_config config = phys_particle_config());
		custom::Vector3 getDirection();
		void addForce(custom::Vector3 f);
		void step(double dt) override;
	protected:
		physx::PxRigidDynamic* dynActor = nullptr;
		double max_speed = -1.0;

	};
	class SphereParticle : public DynamicPhysicsObject {
	public:
		SphereParticle(physx::PxScene* s, phys_particle_config config = phys_particle_config());
	protected:
	};
	class Crosshair : public DynamicPhysicsObject {
	public:
		Crosshair(physx::PxScene* s, phys_particle_config config = phys_particle_config());
	};
	class Wall : public StaticPhysicsObject {
	public:
		Wall(physx::PxScene* s, phys_particle_config config = phys_particle_config(), bool vert = true);
	};
}