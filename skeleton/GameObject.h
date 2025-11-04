#pragma once
#include "RenderUtils.hpp"
#include <memory>
#include <list>
struct particle_config {
	custom::Vector3 position;
	custom::Vector3 velocity;
	double lifetime = 1.0;
	double mass;
	double size = 10;
	Vector4 color{ 1, 1, 1, 1 };
	void operator=(particle_config& other) {
		position = other.position;
		velocity = other.velocity;
		lifetime = other.lifetime;
		mass = other.mass;
		color = other.color;
	}
};
struct projectile_config {
	particle_config p_config;
	double speed;
	double gravity = GRAVITY;
	void operator=(projectile_config& other) {
		p_config = other.p_config;
		speed = other.speed;
		gravity = other.gravity;
	}
};
class GameObject {
public:
	GameObject() {};
	~GameObject() { for (auto c : children) if (c) delete c; };
	inline void add_child(GameObject* p) { children.push_back(p); p->set_parent(this); }
	inline void set_parent(GameObject* p) { parent = p; }
	virtual void step(double t);
	virtual void translate(custom::Vector3 t);
	inline bool is_alive() const { return alive; }
	inline custom::Vector3 get_position() const { return custom::Vector3::convert(pose.p); }
	inline void set_position(custom::Vector3 tr) { translate(custom::Vector3::convert(pose.p) - tr); }
	inline custom::Vector3 get_direction() const { auto aux = vel; return aux; };
protected:
	custom::Vector3 vel;
	custom::Vector3 accel;
	physx::PxTransform pose;
	double lifetime;
	bool alive = true;
	bool timed = false;
	std::list<GameObject*> children;
	GameObject* parent;
};
class Particle: public GameObject {
public:
	Particle(particle_config c);
	~Particle();
	virtual void step(double t) override;
	virtual void integrate(double t);
	inline void set_accel(custom::Vector3 acc) { accel = acc; }
	inline void set_damping(float d) { damping = d; }
	inline void set_color(Vector4 c) { _renderItem->color = c; };
	inline double get_mass() const { return mass_simulated; }
	inline double get_mass_inverse() const { return mass_inverse; }
	inline void add_force(custom::Vector3 force) { force_accum += force; };

protected:
	double damping = 0.999;
	RenderItem* _renderItem;
	double mass_simulated;
	double mass_inverse;
	double gravity_simulated;
	double speed_simulated;
	double max_speed = -1.0;

	custom::Vector3 force_accum = custom::Vector3::blank();

};

class Projectile : public Particle {
public:
	Projectile(projectile_config c);
	//~Projectile() = default;
	void integrate(double t) override;
protected:
	double mass_real;
	double gravity_real;
	double speed_real;
	void update_gravity_s();
	void update_mass_s();
	double speed_factor();
	custom::Vector3 gravAccel = custom::Vector3::blank();
};