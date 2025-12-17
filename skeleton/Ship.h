#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
#include "ForceGenerator.h"
enum Direction
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
	NONE = 4
};
custom::Vector3 dirs[] = {
	custom::Vector3(0,1.0,0),
	custom::Vector3(0,-1.0,0),
	custom::Vector3(0,0,+1.0),
	custom::Vector3(0,0,-1.0),
	custom::Vector3::blank()
};
class Ship : public Particle {
public:
	Ship(particle_config p_c):Particle(p_c) {
		timed = false;

		firing_system = new ParticleSystem();
		projectile_config proj1{
		{
			custom::Vector3::blank(),
			custom::Vector3(-1000,0,0),
			1.0,
			1.0,
			10.0,
			GRAVITY,
			{1,0,0,1}
		},
		10000 //Speed
		};
		gen_config gen1(custom::Vector3::convert(pose.p), 1,
			distribution::NORMAL);
		firing_system->add_gen(new ProjectileGenerator(gen1,proj1)); //Primary fire
		particle_config exhaust{
			custom::Vector3::blank(),
			custom::Vector3(300.0,0.0,0.0),
			0.3,
			0.1,
			3.0,
			0.0,
			{0.5,0.5,0.5,0.5}
		};
		gen_config exhgen( custom::Vector3::convert(pose.p),1,UNIFORM,200.0);
		firing_system->add_gen(new TimedParticleGenerator(exhgen, exhaust, 0.1));


		blast_system = new ParticleSystem();
		particle_config proj2{
			custom::Vector3::blank(),
			custom::Vector3::blank(),
			1.0,
			0.1,
			3,
			0.0,
			{0,0,1,1}
		};
		gen_config gen2(custom::Vector3::convert(pose.p), 200,
			distribution::UNIFORM);
		blast_system->add_gen(new ParticleGenerator(gen2,proj2));

		bullet_drag = new WindGen(custom::Vector3::convert(pose.p), custom::Vector3(10, 0, 0));
		firing_system->add_force(bullet_drag);
		drag = new WindGen(custom::Vector3::convert(pose.p), custom::Vector3::blank(), 0.02);
		max_speed = 200.0;
	}
	~Ship() {
		delete firing_system;
		delete blast_system;
		delete drag;
	}
	inline void step(double dt) override {
		firing_system->step(dt);
		blast_system->step(dt);
		//std::cout << vel.getX() << " " << vel.getY() << " " << vel.getZ() << "\n";
		if (vel.mod() < stop_threshold) {
			vel = custom::Vector3::blank();
			accel = custom::Vector3::blank();
		}
		//drag->redirect(vel * -1);
		drag->apply_force(this);
		Particle::step(dt); 
	}
	inline void set_accel(Direction d) {
		custom::Vector3 force = dirs[d] * move_intensity;
		//std::cout << force.getX() << " " << force.getY() << " " << force.getZ() << "\n";
		add_force(force);
	}
	inline void fire() {
		firing_system->fire_at(0);
	}
	inline void blast() {
		blast_system->fire_at(0);
		blast_system->add_force(new ExplosionGen(custom::Vector3::convert(pose.p), 50.0));
	}
	virtual void translate(custom::Vector3 t) override {
		firing_system->translate(t);
		blast_system->translate(t);
		drag->translate(t);
		Particle::translate(t);
	}

	inline void toggle_drag() { drag->set_active(!drag->get_active()); }
	inline void toggle_bullet_drag() { bullet_drag->set_active(!bullet_drag->get_active()); }
protected:
	double move_intensity = 10000.0;
	ParticleSystem* firing_system;
	ParticleSystem* blast_system;
	WindGen* bullet_drag;
	WindGen* drag;
	double stop_threshold = 0.5;
};