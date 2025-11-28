#include "SpringGen.h"

SpringGen::SpringGen(double k, double resting_length, Particle* other)
{
	_k = k;
	_resting_length = resting_length;
	_other = other;
}

void SpringGen::apply_force(Particle* particle)
{
	custom::Vector3 vec = _other->get_position() - particle->get_position();
	custom::Vector3 force;

	const float length = vec.normalized().mod();
	const float delta_x = length - _resting_length;

	force = vec * delta_x * _k;

	particle->add_force(force);
}

AnchoredSpringGen::AnchoredSpringGen(double k, double resting_length, custom::Vector3 anchor_pos):SpringGen(k, resting_length, nullptr)
{
	auto pconf = particle_config{ anchor_pos, custom::Vector3(0, 0, 0) };
	_other = new Particle(pconf);
}

AnchoredSpringGen::~AnchoredSpringGen()
{
	delete _other;
}
