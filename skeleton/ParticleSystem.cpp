#include "ParticleSystem.h"
#include "Distributions.h"
#include "ForceGenerator.h"
ParticleGenerator::ParticleGenerator(gen_config g) : 
pr_config(g.pr_config),
velocity_avg(g.velocity_avg),
gen_time(g.gen_time),
gen_count(g.gen_count),
dist(g.dist),
interest_range(g.interest_range)
{
	time_since_gen = 0.0;
	pose.p = g.origin.converted();
}

ParticleGenerator::~ParticleGenerator()
{
	for (auto p : particles) {
		if (p) delete p;
	}
}

void ParticleGenerator::step(double t)
{
	//TODO: Cambiar vector a lista, esto es atroz.
	auto it = particles.begin();
	while (it != particles.end()) {
		if (!(*it)->is_alive() || ((*it)->get_position() - get_position()).mod() > interest_range){
			delete* it;
			it = particles.erase(it);
		}
		else {
			(*it)->step(t);
			it++;
		}
	}
	GameObject::step(t);
}

void ParticleGenerator::generate(){
	for (int i = 0; i < gen_count;i++) {
		Projectile* aux;
		projectile_config conf_aux = pr_config;
		conf_aux.p_config.position += custom::Vector3::convert(pose.p);
		custom::Vector3 vec_aux;
		switch (dist) {
		case NORMAL:
			vec_aux = custom::Vector3(Distributions::next_normal(), Distributions::next_normal(), Distributions::next_normal());
			conf_aux.p_config.position += vec_aux;
			aux = new Projectile(conf_aux);
			break;
		case UNIFORM:
			vec_aux = custom::Vector3(Distributions::next_uniform(), Distributions::next_uniform(), Distributions::next_uniform());
			conf_aux.p_config.position += vec_aux - custom::Vector3(0.5,0.5,0.5);
			aux = new Projectile(conf_aux);
			break;
		default:
			aux = new Projectile(conf_aux);
			break;
		}
		particles.push_back(aux);
	}
}

void ParticleGenerator::update_direction(custom::Vector3 dir)
{
	pr_config.p_config.velocity = dir;
}

void TimedParticleGenerator::step(double t)
{
	ParticleGenerator::step(t);
	time_since_gen += t;
	if (time_since_gen > gen_time) {
		for (int i = 0;i < gen_count;i++) generate();
		time_since_gen = 0.0;
	}
}

ParticleSystem::~ParticleSystem()
{
	for (auto gen : gens) if (gen) delete gen;
}

void ParticleSystem::step(double t)
{
	auto it = gens.begin();
	while (it != gens.end()) {
		if (!(*it)->is_alive()) {
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
		if (!(*it2)->is_alive()) {
			delete* it2;
			it2 = forces.erase(it2);
		}
		else {
			(*it2)->step(t);
			for (auto &g : gens) {
				auto parts = g->get_particles();
				for (auto p : parts) {
					(*it2)->apply_force(p);
				}
			}
			it2++;
		}
	}
	/*for (auto f : forces) {
		for (auto g : gens) {
			auto parts = g->get_particles();
			for (auto p : parts) {
				f->apply_force(p);
			}
		}
	}*/
	/*auto it2 = children.begin();
	while (it2 != children.end()) {
		if (!(*it2)->is_alive() || ((*it)->get_position() - position).mod() > interest_range) {
			delete* it2;
			it2 = children.erase(it2);
		}
		else {
			(*it2)->step(t);
			it2++;
		}
	}*/
	GameObject::step(t);
}

void ParticleSystem::translate(custom::Vector3 t)
{
	for (auto f : forces) {
		if (f) f->translate(t);
	}
	for (auto g : gens) {
		if (g) g->translate(t);
	}
	GameObject::translate(t);
}
