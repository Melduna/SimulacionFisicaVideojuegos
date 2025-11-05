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
class Ship : public Projectile {
public:
	Ship(projectile_config p_c):Projectile(p_c) {
		timed = false;

		firing_system = new ParticleSystem();
		projectile_config proj1{
		{
			custom::Vector3::blank(),
			custom::Vector3(-10,0,0),
			1.0,
			1.0,
			5,
			{1,0,0,1}
		},
		300 //Speed
		};
		gen_config gen1(proj1, custom::Vector3::convert(pose.p), custom::Vector3(100,0,0), 1,
			distribution::NORMAL);
		firing_system->add_gen(new ParticleGenerator(gen1)); //Primary fire

		blast_system = new ParticleSystem();
		projectile_config proj2{
			custom::Vector3::blank(),
			custom::Vector3::blank(),
			1.0,
			1.0,
			3,
			{0,0,1,1},
			100.0,
			0.0
		};
		gen_config gen2(proj2, custom::Vector3::convert(pose.p), custom::Vector3::blank(), 200,
			distribution::UNIFORM);
		blast_system->add_gen(new ParticleGenerator(gen2));

		firing_system->add_force(new WindGen(custom::Vector3::convert(pose.p), custom::Vector3(10, 0, 0)));
		drag = new WindGen(custom::Vector3::convert(pose.p),custom::Vector3::blank(), 0.05);
		max_speed = 3.0;
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
		drag->redirect(vel * -1);
		drag->apply_force(this);
		Projectile::step(dt); 
	}
	inline void set_accel(Direction d) {
		add_force(dirs[d] * move_intensity);
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
		Projectile::translate(t);
	}

protected:
	double move_intensity = 50.0;
	ParticleSystem* firing_system;
	ParticleSystem* blast_system;
	WindGen* drag;
	double stop_threshold = 0.03;
};