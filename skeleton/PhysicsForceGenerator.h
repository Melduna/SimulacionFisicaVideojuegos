#pragma once
#include "PhysicsObject.h"
#include "PhysicsParticleSystem.h"
namespace physics {
	class PhysicsForceGenerator : public DynamicPhysicsObject {
	public:
		PhysicsForceGenerator(physx::PxScene* s, phys_particle_config config = phys_particle_config()) : DynamicPhysicsObject(s,config) {
			dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
		};
		virtual void applyForce(DynamicPhysicsObject*) = 0;
		virtual void step(double dt) {};
		inline bool get_active() const { return active; }
		inline void set_active(bool a) { active = a; }
	protected:
		bool active = true;
	};
	//class GravityGen :public ForceGenerator {
	//public:
	//	inline GravityGen(double g = -9.8) { gravity = custom::Vector3(0, g, 0); };
	//	void apply_force(Particle*) override;
	//protected:
	//	custom::Vector3 gravity;
	//};
	class PhysicsWindGen :public PhysicsForceGenerator {
	public:
		inline PhysicsWindGen(physx::PxScene* s, custom::Vector3 center, custom::Vector3 d = custom::Vector3::blank(), double re = 0.0001, double t = 0.0, double ra = 50.0);
		void applyForce(DynamicPhysicsObject*) override;
		inline void redirect(custom::Vector3 v) { direction = v.normalized(); };
	protected:
		custom::Vector3 direction;
		double resistance;
		double turbulence;
		double radius;
	};
	//class PhysicsVortexGen :public PhysicsForceGenerator {
	//public:
	//	inline PhysicsVortexGen(physx::PxScene* s, custom::Vector3 p, double r = 50.0, double i = 1.0);
	//	void apply_force(Particle*) override;
	//protected:
	//	double radius;
	//	double intensity;
	//};
	class PhysicsExplosionGen :public PhysicsForceGenerator {
	public:
		inline PhysicsExplosionGen(physx::PxScene* s, custom::Vector3 p, double i = 50.0, double t = 5.0, double r = 50.0, double R = 200.0);
		void step(double t) override { PhysicsObject::step(t); }
		void applyForce(DynamicPhysicsObject*) override;
	protected:
		custom::Vector3 startpos;
		double max_time;
		double start_radius;
		double end_radius;
		double intensity = 50.0;
		const double constant = 1.0;
	};


	class PhysicsSpringGen : public PhysicsForceGenerator {
	public:
		PhysicsSpringGen(physx::PxScene* s, double k, double resting_length, DynamicPhysicsObject* other);

		virtual void applyForce(DynamicPhysicsObject*) override;

		inline void set_k(double k) { _k = k; }
	protected:
		double _k;
		double _resting_length;
		DynamicPhysicsObject* _other;
	};
	class AnchoredSpringGen : public PhysicsSpringGen {
	public:
		AnchoredSpringGen(physx::PxScene* s, double k, double resting_length, custom::Vector3 anchor_pos);
		~AnchoredSpringGen();
	};
}