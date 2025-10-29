#pragma once
#include "GameObject.h"
#include <random>
enum distribution {
	UNIFORM,
	NORMAL,
	CONSTANT
};

struct gen_config {
	projectile_config pr_config;
	custom::Vector3 origin;
	custom::Vector3 velocity_avg;
	int gen_count;
	distribution dist;
	double gen_time;
	double interest_range;
	gen_config(projectile_config pconf,
		custom::Vector3 o,
		custom::Vector3 v_avg,
		int g_c = 1,
		distribution dt = NORMAL,
		double g_t = 1.0,
		double i_r = 1000.0) {
		pr_config = pconf;
		origin = o;
		velocity_avg = v_avg;
		gen_count = g_c;
		dist = dt;
		gen_time = g_t;
		interest_range = i_r;
	};
};

class ParticleGenerator : public GameObject {
public:
	ParticleGenerator(gen_config g);
	~ParticleGenerator();
	virtual void step(double t);
	GameObject* generate();
	GameObject* generate(custom::Vector3 dir);
protected:
	projectile_config pr_config;
	std::list<Particle*> particles;
	custom::Vector3 velocity_avg;
	double gen_time;
	double time_since_gen;
	int gen_count;
	distribution dist;
	double interest_range;
};

class TimedParticleGenerator : public ParticleGenerator {
public:
	TimedParticleGenerator(gen_config g) : ParticleGenerator(g) {}
	void step(double t) override;
};

class ParticleSystem : public GameObject {
public:
	ParticleSystem() {}
	~ParticleSystem();
	inline void add_gen(ParticleGenerator* p) { gens.push_back(p); }
	inline void fire() { gens.front()->generate(); }
	inline void fire(custom::Vector3 c) { gens.front()->generate(c); }
	void step(double t);
protected:
	std::list<ParticleGenerator*> gens;
};