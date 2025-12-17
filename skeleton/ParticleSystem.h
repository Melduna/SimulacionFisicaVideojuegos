#pragma once
#include "GameObject.h"
#include <random>
class ForceGenerator;
enum distribution {
	UNIFORM,
	NORMAL,
	CONSTANT,
};

struct gen_config {
	custom::Vector3 origin;
	int gen_count;
	distribution dist;
	double interest_range;
	gen_config(custom::Vector3 o,
		int g_c = 1,
		distribution dt = NORMAL,
		double i_r = 1000.0) {
		origin = o;
		gen_count = g_c;
		dist = dt;
		interest_range = i_r;
	};
};

class ForceGenerator;
class ParticleGenerator : public GameObject {
public:
	ParticleGenerator(gen_config g, particle_config pa);
	~ParticleGenerator();
	virtual void step(double t);
	virtual void generate();
	virtual void update_direction(custom::Vector3 dir);
	inline std::list<Particle*>& get_particles() { return particles; };
protected:
	particle_config pa_config;
	std::list<Particle*> particles;
	double gen_time;
	double time_since_gen;
	int gen_count;
	distribution dist;
	double interest_range;
};
class ProjectileGenerator : public ParticleGenerator {
public:
	ProjectileGenerator(gen_config g, projectile_config pr);
	void update_direction(custom::Vector3 dir) override;
	void generate() override;
protected:
	projectile_config pr_config;
};

class TimedParticleGenerator : public ParticleGenerator {
public:
	TimedParticleGenerator(gen_config g, particle_config pa, double t) : ParticleGenerator(g, pa),gen_time(t) {}
	void step(double t) override;
protected:
	double gen_time;
};

class ParticleSystem : public GameObject {
public:
	ParticleSystem() {}
	ParticleSystem(gen_config g);
	~ParticleSystem();
	inline void add_gen(ParticleGenerator* p) { gens.push_back(p); }
	inline void add_force(ForceGenerator* p) { forces.push_back(p); }
	inline void fire() { gens.front()->generate(); }
	inline void fire_at(int index) {
		int i = 0;
		auto it = gens.begin();
		while (it != gens.end() && i < index) {
			i++;
		}
		if (it != gens.end()) (*it)->generate();
	}
	inline std::list<ParticleGenerator*>& get_gens() { return gens; };
	void step(double t);
	void translate(custom::Vector3 t) override;
protected:
	std::list<ParticleGenerator*> gens;
	std::list<ForceGenerator*> forces;
};