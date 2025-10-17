#pragma once
#include "RenderUtils.hpp"
#include <memory>
struct particle_config {
	custom::Vector3 position;
	custom::Vector3 velocity;
	double lifetime = 1.0;
};
struct projectile_config {
	particle_config p_config;
	double mass;
	double speed;
	double gravity = GRAVITY;
};
class Particle {
public:
	Particle(particle_config c);
	~Particle();

	virtual void integrate(double t);
	inline void set_accel(custom::Vector3 acc) { accel = acc; }
	inline void set_damping(float d) { damping = d; }
	inline bool is_alive() { return alive; }
protected:
	custom::Vector3 vel;
	custom::Vector3 accel;
	double damping = 0.999;
	physx::PxTransform pose;
	RenderItem* _renderItem;
	double lifetime;
	double remaining_lifetime;
	bool alive = true;
};

class Projectile : public Particle {
public:
	Projectile(projectile_config c);
	//~Projectile() = default;
	//inline void add_force(custom::Vector3 force) { accel += force * (1.0 / mass_simulated); };
	//void integrate(double t) override;
protected:
	double mass_real;
	double mass_simulated;
	double gravity_real;
	double gravity_simulated;
	double speed_real;
	double speed_simulated;
	void update_gravity_s();
	void update_mass_s();
	double speed_factor();
	custom::Vector3 gravAccel = custom::Vector3(0, 0, 0);
};
