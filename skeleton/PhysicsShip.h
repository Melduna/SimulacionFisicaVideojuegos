#pragma once
#include "PhysicsObject.h"
namespace physics {
	class PhysicsShip : public SphereParticle {
		PhysicsShip(physx::PxScene* s, phys_particle_config config = phys_particle_config());
	};
}