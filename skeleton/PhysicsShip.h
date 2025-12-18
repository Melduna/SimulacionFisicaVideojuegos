#pragma once
#include "PhysicsForceGenerator.h"
class GameManager;

namespace physics {
	enum PhysDirection
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		NONE = 4
	};
	static custom::Vector3 physdirs[]{
		custom::Vector3(0,1.0,0),
		custom::Vector3(0,-1.0,0),
		custom::Vector3(0,0,+1.0),
		custom::Vector3(0,0,-1.0),
		custom::Vector3::blank()
	};
	class PhysicsShip : public SphereParticle {
	public:
		PhysicsShip(physx::PxScene* s, phys_particle_config config = phys_particle_config(), double sf = 1.0, double df = 1.0, double r = 1.0);
	protected:
		GameManager* gameManager = nullptr;
	};
	class PlayerShip : public PhysicsShip {
	public:
		PlayerShip(physx::PxScene* s, phys_particle_config config = phys_particle_config());
		~PlayerShip();
		void step(double dt) override;
		inline void set_accel(PhysDirection d) {
			custom::Vector3 force = physdirs[d] * move_intensity;
			//std::cout << force.getX() << " " << force.getY() << " " << force.getZ() << "\n";
			addForce(force);
		}
		void translate(custom::Vector3 v) override;
		void fire();
		//inline void blast() {
		//	blast_system->fire_at(0);
		//	blast_system->add_force(new PhysicsExplosionGen(scene, custom::Vector3::convert(actor->getGlobalPose().p), 50.0));
		//}
		inline void toggle_drag() { drag->set_active(!drag->get_active()); }
		void reset() override;
		void reset_stage_1();
		void reset_stage_2();
		//inline void toggle_bullet_drag() { bullet_drag->set_active(!bullet_drag->get_active()); }
	protected:
		double move_intensity = 500.0;
		PhysicsParticleSystem* firing_system = nullptr;
		physics::Crosshair* crosshair = nullptr;
		PhysicsSpringGen* spring = nullptr;
		//PhysicsParticleSystem* blast_system;
		//PhysicsWindGen* bullet_drag;
		PhysicsWindGen* drag = nullptr;
		double stop_threshold = 0.005;
	};
	class EnemyShip : public PhysicsShip {
	public:
		EnemyShip(physx::PxScene* s, phys_particle_config config = phys_particle_config(), int i = 0);
		~EnemyShip();
		void step(double dt) override;
		bool die();
	protected:
		PhysicsParticleSystem* blast;
		int index;
	};
}