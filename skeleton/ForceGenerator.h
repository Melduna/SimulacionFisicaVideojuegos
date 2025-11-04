#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
class ForceGenerator : public GameObject {
public:
	ForceGenerator() = default;
	~ForceGenerator() = default;
	virtual void apply_force(Projectile*) = 0;
	virtual void step(double dt) {};
	inline void apply_on_targets(ParticleSystem* s) {
		auto& aux = s->get_gens();
		for (auto g : aux) {
			auto& parts = g->get_particles();
			for (auto p : parts) {
				apply_force(p);
			}
		}
	}
protected:
};
class GravityGen :public ForceGenerator {
public:
	inline GravityGen(double g = -9.8) { gravity = custom::Vector3(0, g, 0); };
	void apply_force(Projectile*) override;
protected:
	custom::Vector3 gravity;
};
class WindGen :public ForceGenerator {
public:
	inline WindGen(custom::Vector3 center, custom::Vector3 d = custom::Vector3::blank(), double re = 0.0001, double t = 0.0, double ra = 50.0) :resistance(re), turbulence(t), radius(ra) { pose.p = center.converted(); direction = d.normalized(); };
	void apply_force(Projectile*) override;
	inline void redirect(custom::Vector3 v) { direction = v.normalized(); };
protected:
	custom::Vector3 direction;
	double resistance;
	double turbulence;
	double radius;
};
class VortexGen :public ForceGenerator {
public:
	inline VortexGen(custom::Vector3 p) { pose.p = p.converted(); }
	void apply_force(Projectile*) override;
protected:
};
class ExplosionGen :public ForceGenerator {
public:
	inline ExplosionGen(custom::Vector3 p) { pose.p = p.converted(); }
	void apply_force(Projectile*) override;
	void step(double dt) override { timer += dt; };
protected:
	double radius = 100.0;
	double intensity = 50.0;
	const double constant = 1.0;
	double timer = 0.0;
};