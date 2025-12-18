#include "PhysicsParticleSystem.h"
#include "PhysicsForceGenerator.h"
physics::PhysicsParticleGenerator::PhysicsParticleGenerator(physx::PxScene* s, phys_gen_config g, phys_particle_config pa):DynamicPhysicsObject(s,physics::phys_particle_config(g.origin)),pa_config(pa)
{
	gen_count = g.gen_count;
	dist = g.dist;
	interest_range = g.interest_range;
	dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	ghost = g.ghost;
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
		if (!ghost) aux = new physics::SphereParticle(scene,conf_aux);
		else aux = new physics::GhostSphereParticle(scene, conf_aux);

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
	dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}

physics::PhysicsParticleSystem::~PhysicsParticleSystem()
{
	for (auto p : forces) {
		if (p) delete p;
	}
	for (auto p : gens) {
		if (p) delete p;
	}
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
					(*it2)->applyForce(p);
				}
			}
			it2++;
		}
	}
}

void physics::PhysicsParticleSystem::translate(custom::Vector3 v)
{
	DynamicPhysicsObject::translate(v);
	for (auto f : forces) {
		if (f) f->translate(v);
	}
	for (auto g : gens) {
		if (g) g->translate(v);
	}
}
