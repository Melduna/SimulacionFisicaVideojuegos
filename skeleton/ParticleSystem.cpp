#include "ParticleSystem.h"
#include "Distributions.h"
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

GameObject* ParticleGenerator::generate(){
	Projectile* aux;
	projectile_config conf_aux = pr_config;
	custom::Vector3 vec_aux;
	switch (dist) {
	case NORMAL:
		vec_aux = custom::Vector3(Distributions::next_normal(), Distributions::next_normal(), Distributions::next_normal());
		conf_aux.p_config.position+=vec_aux;
		aux = new Projectile(conf_aux);
		break;
	case UNIFORM:
		vec_aux = custom::Vector3(Distributions::next_uniform(), Distributions::next_uniform(), Distributions::next_uniform());
		conf_aux.p_config.position += vec_aux;
		aux = new Projectile(conf_aux);
		break;
	default:
		aux = new Projectile(conf_aux);
		break;
	}
	particles.push_back(aux);
	return aux;
}

GameObject* ParticleGenerator::generate(custom::Vector3 dir)
{
	Projectile* aux;
	projectile_config conf_aux = pr_config;
	conf_aux.p_config.velocity = dir;
	custom::Vector3 vec_aux;
	switch (dist) {
	case NORMAL:
		vec_aux = custom::Vector3(Distributions::next_normal(), Distributions::next_normal(), Distributions::next_normal());
		conf_aux.p_config.position += vec_aux;
		aux = new Projectile(conf_aux);
		break;
	case UNIFORM:
		vec_aux = custom::Vector3(Distributions::next_uniform(), Distributions::next_uniform(), Distributions::next_uniform());
		conf_aux.p_config.position += vec_aux;
		aux = new Projectile(conf_aux);
		break;
	default:
		aux = new Projectile(conf_aux);
		break;
	}
	particles.push_back(aux);
	return aux;
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
