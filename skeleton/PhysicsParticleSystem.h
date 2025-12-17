#pragma once
#include "Distributions.h"
#include "Vector3.h"
#include "PhysicsObject.h"
#include "GameObject.h"
namespace physics {
	enum distribution {
		UNIFORM,
		NORMAL,
		CONSTANT,
	};
	struct phys_gen_config {
		custom::Vector3 origin;
		int gen_count;
		distribution dist;
		double interest_range;
		phys_gen_config(custom::Vector3 o = custom::Vector3::blank(),
			int g_c = 1,
			distribution dt = NORMAL,
			double i_r = 1000.0) {
			origin = o;
			gen_count = g_c;
			dist = dt;
			interest_range = i_r;
		};
	};
	class PhysicsForceGenerator;
	class PhysicsParticleGenerator : public DynamicPhysicsObject {
	public:
		PhysicsParticleGenerator(physx::PxScene* s, phys_gen_config g = phys_gen_config(), phys_particle_config pa = phys_particle_config());
		~PhysicsParticleGenerator();
		virtual void step(double t) override;
		virtual void generate();
		virtual void updateDirection(custom::Vector3 dir);
		inline std::list<DynamicPhysicsObject*>& getParticles() { return particles; };
	protected:
		phys_particle_config pa_config;
		std::list<DynamicPhysicsObject*> particles;
		double time_since_gen;
		int gen_count;
		distribution dist;
		double interest_range;
	};

	class TimedPhysicsParticleGenerator : public PhysicsParticleGenerator {
	public:
		TimedPhysicsParticleGenerator(physx::PxScene* s, phys_gen_config g = phys_gen_config(), phys_particle_config pa = phys_particle_config(), double t = 5.0) : PhysicsParticleGenerator(s, g, pa), gen_time(t) {}
		void step(double t) override;
	protected:
		double gen_time;
	};
	class PhysicsParticleSystem : public DynamicPhysicsObject {
	public:
		PhysicsParticleSystem(physx::PxScene* s);
		~PhysicsParticleSystem() {};
		inline void add_gen(PhysicsParticleGenerator* p) { gens.push_back(p); }
		inline void add_force(PhysicsForceGenerator* p) { forces.push_back(p); }
		inline void fire() { gens.front()->generate(); }
		inline void fire_at(int index) {
			int i = 0;
			auto it = gens.begin();
			while (it != gens.end() && i < index) {
				i++;
			}
			if (it != gens.end()) (*it)->generate();
		}
		inline std::list<PhysicsParticleGenerator*>& get_gens() { return gens; };
		void step(double t) override;
		//void translate(custom::Vector3 t) override;
	protected:
		std::list<PhysicsParticleGenerator*> gens;
		std::list<PhysicsForceGenerator*> forces;
	};
}