#include "PhysicsShip.h"

physics::PhysicsShip::PhysicsShip(physx::PxScene* s, phys_particle_config config):
	SphereParticle(s, config)
{
	dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}
