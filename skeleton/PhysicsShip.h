#pragma once
#include "PhysicsObject.h"
#include "PhysicsParticleSystem.h"
#include "PhysicsForceGenerator.h"
namespace physics {
	enum Direction
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		NONE = 4
	};
	custom::Vector3 dirs[] = {
		custom::Vector3(0,1.0,0),
		custom::Vector3(0,-1.0,0),
		custom::Vector3(0,0,+1.0),
		custom::Vector3(0,0,-1.0),
		custom::Vector3::blank()
	};
	class PhysicsShip : public SphereParticle {
	public:
		PhysicsShip(physx::PxScene* s, phys_particle_config config = phys_particle_config());
	protected:
	};
	class PlayerShip : public PhysicsShip {
	public:
		PlayerShip(physx::PxScene* s, phys_particle_config config = phys_particle_config());
		~PlayerShip();
		void step(double dt) override;
		inline void set_accel(Direction d) {
			custom::Vector3 force = dirs[d] * move_intensity;
			//std::cout << force.getX() << " " << force.getY() << " " << force.getZ() << "\n";
			addForce(force);
		}
		void translate(custom::Vector3 v) override;
		inline void fire() {
			firing_system->fire_at(0);
		}
		inline void blast() {
			blast_system->fire_at(0);
			blast_system->add_force(new PhysicsExplosionGen(scene,custom::Vector3::convert(actor->getGlobalPose().p), 50.0));
		}
		inline void toggle_drag() { drag->set_active(!drag->get_active()); }
		//inline void toggle_bullet_drag() { bullet_drag->set_active(!bullet_drag->get_active()); }
	protected:
		double move_intensity = 10000.0;
		PhysicsParticleSystem* firing_system;
		PhysicsParticleSystem* blast_system;
		//PhysicsWindGen* bullet_drag;
		PhysicsWindGen* drag;
		double stop_threshold = 0.5;
	};
}