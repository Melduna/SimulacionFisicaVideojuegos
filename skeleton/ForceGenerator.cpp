#include "ForceGenerator.h"

void GravityGen::apply_force(Projectile* p)
{
	double mass_inverse = p->get_mass_inverse();
	p->add_force(gravity * mass_inverse);
}

void WindGen::apply_force(Projectile* p)
{
	auto vel = p->get_direction();
	custom::Vector3 force = (direction - vel)*resistance + ((direction - vel)*((direction - vel).mod())*turbulence);
	p->add_force(force);

}
