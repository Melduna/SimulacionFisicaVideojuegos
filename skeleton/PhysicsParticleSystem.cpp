#include "PhysicsParticleSystem.h"
#include "PhysicsForceGenerator.h"
physics::PhysicsParticleGenerator::PhysicsParticleGenerator(physx::PxScene* s, phys_gen_config g, phys_particle_config pa):DynamicPhysicsObject(s)
{
	dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

}

physics::PhysicsParticleGenerator::~PhysicsParticleGenerator()
{
	for (auto p : particles) {
		if (p) delete p;
	}
}

void physics::PhysicsParticleGenerator::step(double t)
{
	auto it = particles.begin();
	while (it != particles.end()) {
		if (!(*it)->isAlive() || ((*it)->getPosition() - getPosition()).mod() > interest_range) {
			delete* it;
			it = particles.erase(it);
		}
		else {
			(*it)->step(t);
			it++;
		}
	}
	PhysicsObject::step(t);
}

void physics::PhysicsParticleGenerator::generate()
{
	for (int i = 0; i < gen_count;i++) {
		DynamicPhysicsObject* aux;
		phys_particle_config conf_aux = pa_config;
		conf_aux.position += getPosition();
		custom::Vector3 vec_aux = custom::Vector3::blank();
		if (dist == NORMAL)
			vec_aux = custom::Vector3(Distributions::next_normal(), Distributions::next_normal(), Distributions::next_normal());
		else if (dist == UNIFORM)
			vec_aux = custom::Vector3(Distributions::next_uniform(), Distributions::next_uniform(), Distributions::next_uniform());
		conf_aux.position += vec_aux - custom::Vector3(0.5, 0.5, 0.5);
		aux = new physics::DynamicPhysicsObject(scene,conf_aux);

		particles.push_back(aux);
	}
}

void physics::PhysicsParticleGenerator::updateDirection(custom::Vector3 dir)
{
	pa_config.velocity = dir;
}

void physics::TimedPhysicsParticleGenerator::step(double t)
{
	PhysicsParticleGenerator::step(t);
	time_since_gen += t;
	if (time_since_gen > gen_time) {
		for (int i = 0;i < gen_count;i++) generate();
		time_since_gen = 0.0;
	}
}

physics::PhysicsParticleSystem::PhysicsParticleSystem(physx::PxScene* s):DynamicPhysicsObject(s)
{
}

void physics::PhysicsParticleSystem::step(double t)
{
	auto it = gens.begin();
	while (it != gens.end()) {
		if (!(*it)->isAlive()) {
			delete* it;
			it = gens.erase(it);
		}
		else {
			(*it)->step(t);
			it++;
		}
	}
	auto it2 = forces.begin();
	while (it2 != forces.end()) {
		if (!(*it2)->isAlive()) {
			delete* it2;
			it2 = forces.erase(it2);
		}
		else {
			(*it2)->step(t);
			for (auto& g : gens) {
				auto parts = g->getParticles();
				for (auto p : parts) {
					(*it2)->apply_force(p);
				}
			}
			it2++;
		}
	}
}
