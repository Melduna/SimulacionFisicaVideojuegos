#pragma once
#include "ForceGenerator.h"
class BuoyancyGen : public ForceGenerator {
	BuoyancyGen(float h, float v, float d);
	~BuoyancyGen();
	virtual void apply_force(Particle* p) override;
protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_part;
};