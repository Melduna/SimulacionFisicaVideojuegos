#pragma once
#include "GameObject.h"
#include <random>
class ForceGenerator;
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

class ForceGenerator;
class ParticleGenerator : public GameObject {
public:
	ParticleGenerator(gen_config g);
	~ParticleGenerator();
	virtual void step(double t);
	GameObject* generate();
	void update_direction(custom::Vector3 dir);
	inline std::list<Projectile*>& get_particles() { return particles; };
protected:
	projectile_config pr_config;
	std::list<Projectile*> particles;
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
	inline void add_force(ForceGenerator* p) { forces.push_back(p); }
	inline void fire() { gens.front()->generate(); }
	inline std::list<ParticleGenerator*>& get_gens() { return gens; };
	void step(double t);
	void translate(custom::Vector3 t) override;
protected:
	std::list<ParticleGenerator*> gens;
	std::list<ForceGenerator*> forces;
};