#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
class ForceGenerator : public GameObject {
public:
	ForceGenerator() = default;
	~ForceGenerator() = default;
	virtual void apply_force(Particle*) = 0;
	virtual void step(double dt) {};
	inline bool get_active() const { return active; }
	inline void set_active(bool a) { active = a; }
protected:
	bool active = true;
};
class GravityGen :public ForceGenerator {
public:
	inline GravityGen(double g = -9.8) { gravity = custom::Vector3(0, g, 0); };
	void apply_force(Particle*) override;
protected:
	custom::Vector3 gravity;
};
class WindGen :public ForceGenerator {
public:
	inline WindGen(custom::Vector3 center, custom::Vector3 d = custom::Vector3::blank(), double re = 0.0001, double t = 0.0, double ra = 50.0) :resistance(re), turbulence(t), radius(ra) { pose.p = center.converted(); direction = d.normalized(); };
	void apply_force(Particle*) override;
	inline void redirect(custom::Vector3 v) { direction = v.normalized(); };
protected:
	custom::Vector3 direction;
	double resistance;
	double turbulence;
	double radius;
};
class VortexGen :public ForceGenerator {
public:
	inline VortexGen(custom::Vector3 p, double r = 50.0, double i = 1.0) :radius(r), intensity(i) { pose.p = p.converted(); }
	void apply_force(Particle*) override;
protected:
	double radius;
	double intensity;
};
class ExplosionGen :public ForceGenerator {
public:
	inline ExplosionGen(custom::Vector3 p, double i = 50.0, double t = 5.0, double r = 50.0, double R = 200.0)
		:start_radius(r),end_radius(R), intensity(i) 
	{
		max_time = lifetime = t;
		startpos = p;
		pose.p = p.converted(); timed = true;
	}
	void step(double t) override { GameObject::step(t); }
	void apply_force(Particle*) override;
protected:
	custom::Vector3 startpos;
	double max_time;
	double start_radius;
	double end_radius;
	double intensity = 50.0;
	const double constant = 1.0;
};