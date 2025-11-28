#include "Buoyancy.h"

BuoyancyGen::BuoyancyGen(float h, float v, float d)
{
	_height = h;
	_volume = v;
	_liquid_density = d;
	particle_config pconf{ custom::Vector3::blank(),custom::Vector3::blank() };
	_liquid_part = new Particle(pconf);
}

BuoyancyGen::~BuoyancyGen()
{
	delete _liquid_part;
}

void BuoyancyGen::apply_force(Particle* p)
{
	float h = p->get_position().getY();
	float h0 = _liquid_part->get_position().getY();	

	
	float immersed;
	if (h - h0 > _height * 0.5) immersed = 0.0;
	else if (h0 - h > _height * 0.5) immersed = 1.0;
	else immersed = (h0 - h) / _height + 0.5;
	custom::Vector3 f(0, _liquid_density * _volume * immersed * _gravity, 0);

	p->add_force(f);
}
