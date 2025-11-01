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
		particle_config pa_c();
		projectile_config pr_c();
	}
	void set_accel(Direction d) {
		add_force(dirs[d]);
	}
	void fire() {
		firing_system->fire();
	}
protected:
	ParticleSystem* firing_system;
};