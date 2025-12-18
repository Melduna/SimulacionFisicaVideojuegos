#include "PhysicsForceGenerator.h"
#define CONST_E 2.71828

physics::PhysicsWindGen::PhysicsWindGen(physx::PxScene* s, custom::Vector3 center, custom::Vector3 d, double re, double t, double ra) :
	PhysicsForceGenerator(s,physics::phys_particle_config(center)),resistance(re), turbulence(t), radius(ra) 
{
	direction = d.normalized();
	//pose.p = center.converted(); direction = d.normalized();
}
void physics::PhysicsWindGen::applyForce(DynamicPhysicsObject* p)
{
	if (active) {
		auto vel = p->getDirection();
		auto distance = (p->getPosition() - getPosition()).mod();
		//std::cout << distance << "\n";
		if (distance <= radius) {
			custom::Vector3 force = ((direction - vel) * resistance + ((direction - vel) * ((direction - vel).mod()) * turbulence));
			//force.print();
			p->addForce(force);
			//std::cout << "Vel: " << vel.getX() << ", " << vel.getY() << ", " << vel.getZ() << "\n" <<
			//	"	Force: " << force.getX()<<", "<<force.getY()<<", "<<force.getZ()<<"\n";
		}

	}
}

;
physics::PhysicsExplosionGen::PhysicsExplosionGen(physx::PxScene* s, custom::Vector3 p, double i, double t, double r, double R)
	:PhysicsForceGenerator(s, physics::phys_particle_config(p)),start_radius(r), end_radius(R), intensity(i)
{
	max_time = lifetime = t;
	startpos = p;
	//pose.p = p.converted(); 
	timed = true;
}

void physics::PhysicsExplosionGen::applyForce(DynamicPhysicsObject* p)
{
	if (active) {
		double time_elapsed = max_time - lifetime;
		double current_radius = (start_radius + end_radius) / max_time * (time_elapsed);
		auto vec = (p->getPosition() - startpos);
		if (vec.mod() <= current_radius) {
			custom::Vector3 force = vec * (intensity / (current_radius * current_radius)) * pow(CONST_E, -(time_elapsed / constant));
			p->addForce(force);
		}
	}
}

physics::PhysicsSpringGen::PhysicsSpringGen(physx::PxScene* s, double k, double resting_length, DynamicPhysicsObject* other):PhysicsForceGenerator(s)
{
	_k = k;
	_resting_length = resting_length;
	_other = other;
}

void physics::PhysicsSpringGen::applyForce(DynamicPhysicsObject* particle)
{
	custom::Vector3 vec = _other->getPosition() - particle->getPosition();
	custom::Vector3 force;

	const float length = vec.normalize();
	const float delta_x = length - _resting_length;

	force = vec * delta_x * _k;

	particle->addForce(force);
}
