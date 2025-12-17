#include "ForceGenerator.h"
#include <cmath>
#include "PhysicsForceGenerator.h"
#define CONST_E 2.71828
void GravityGen::apply_force(Particle* p)
{
	double mass_inverse = p->get_mass_inverse();
	p->add_force(gravity * mass_inverse);
}

void WindGen::apply_force(Particle* p)
{
	if (active) {
		auto vel = p->get_direction();
		auto distance = (p->get_position() - custom::Vector3::convert(pose.p)).mod();
		//std::cout << distance << "\n";
		if (distance <= radius) {
			custom::Vector3 force = ((direction - vel) * resistance + ((direction - vel) * ((direction - vel).mod()) * turbulence));
			//force.print();
			p->add_force(force);
			//std::cout << "Vel: " << vel.getX() << ", " << vel.getY() << ", " << vel.getZ() << "\n" <<
			//	"	Force: " << force.getX()<<", "<<force.getY()<<", "<<force.getZ()<<"\n";
		}

	}
}

void ExplosionGen::apply_force(Particle* p)
{
	if (active) {
		double time_elapsed = max_time - lifetime;
		double current_radius = (start_radius + end_radius) / max_time * (time_elapsed);
		auto vec = (p->get_position() - startpos);
		if (vec.mod() <= current_radius) {
			custom::Vector3 force = vec * (intensity / (current_radius * current_radius)) * pow(CONST_E, -(time_elapsed / constant));
			p->add_force(force);
		}
	}
}

void VortexGen::apply_force(Particle* p)
{
	if (active) {
		auto vec = (p->get_position() - custom::Vector3::convert(pose.p));
		if (vec.mod() <= radius) {
			custom::Vector3 force(vec.getX()*-1, 50-vec.getY(), vec.getZ());
			p->add_force(force*intensity);
		}
	}
}
