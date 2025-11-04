#include "ForceGenerator.h"
#include <cmath>
#define CONST_E 2.71828
void GravityGen::apply_force(Projectile* p)
{
	double mass_inverse = p->get_mass_inverse();
	p->add_force(gravity * mass_inverse);
}

void WindGen::apply_force(Projectile* p)
{
	auto vel = p->get_direction();
	auto distance = (p->get_position() - custom::Vector3::convert(pose.p)).mod();
	//std::cout << distance << "\n";
	if (distance <= radius) {
		custom::Vector3 force = ((direction - vel) * resistance + ((direction - vel) * ((direction - vel).mod()) * turbulence))*0.00001;
		//force.print();
		p->add_force(force);
	}
}

void ExplosionGen::apply_force(Projectile* p)
{
	auto vec = (p->get_direction() - custom::Vector3::convert(pose.p));
	if (vec.mod() <= radius) {
		custom::Vector3 force = vec * (intensity / (radius * radius)) * pow(CONST_E, -(timer / constant));
		p->add_force(force);
	}
}

void VortexGen::apply_force(Projectile*)
{
}
