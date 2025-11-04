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
	custom::Vector3(-1.0,0,0), 
	custom::Vector3(1.0,0,0),
	custom::Vector3(0,0,0)
};
class Ship : Projectile {
public:
	Ship(projectile_config p_c):Projectile(p_c) {
		timed = false;
		projectile_config temp{
		{
			custom::Vector3::convert(pose.p),
			custom::Vector3(-10,0,0),
			1.0,
			1.0,
			5,
			{1,0,0,1}
		},
		300 //Speed
		};
		gen_config temp2(temp, custom::Vector3::convert(pose.p), custom::Vector3(100,0,0), 1,
			distribution::NORMAL);
		firing_system = new ParticleSystem();
		firing_system->add_gen(new ParticleGenerator(temp2));
		firing_system->add_force(new WindGen(custom::Vector3::convert(pose.p), custom::Vector3(10, 0, 0), 0.999));
		
	}
	inline void step(double dt) override { 
		Projectile::step(dt); 
		firing_system->step(dt); }
	inline void set_accel(Direction d) {
		add_force(dirs[d] * move_intensity);
	}
	inline void fire() {
		firing_system->fire();
	}
protected:
	double move_intensity = 5.0;
	ParticleSystem* firing_system;
};