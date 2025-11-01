#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
class ForceGenerator : GameObject {
public:
	ForceGenerator() = default;
	~ForceGenerator() = default;
	virtual void apply_force(Projectile*) = 0;
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
class GravityGen : ForceGenerator {
public:
	inline GravityGen(double g = -9.8) { gravity = custom::Vector3(0, g, 0); };
	void apply_force(Projectile*) override;
protected:
	custom::Vector3 gravity;
};
class WindGen : ForceGenerator {
public:
	inline WindGen(custom::Vector3 d = custom::Vector3(0,0,0), double r = 0.5, double t = 0.0) :direction(d), resistance(r),turbulence(t) {};
	void apply_force(Projectile*) override;
protected:
	custom::Vector3 direction;
	double resistance;
	double turbulence;
};