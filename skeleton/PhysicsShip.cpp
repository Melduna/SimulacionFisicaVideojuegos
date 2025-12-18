#include "PhysicsShip.h"

physics::PhysicsShip::PhysicsShip(physx::PxScene* s, phys_particle_config config):
	SphereParticle(s, config)
{
	dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}

physics::PlayerShip::PlayerShip(physx::PxScene* s, phys_particle_config config):
	PhysicsShip(s,config)
{
	firing_system = new PhysicsParticleSystem(scene);
	phys_particle_config proj1(
		custom::Vector3::blank(),
		custom::Vector3(-1000,0,0),
		1.0,
		1.0,
		{ 1,0,0,1 },
		10.0,
		true
	);
	phys_gen_config gen1(config.position, 1,
		distribution::NORMAL);
	firing_system->add_gen(new PhysicsParticleGenerator(scene,gen1, proj1)); //Primary fire
	//phys_particle_config exhaust(
	//	custom::Vector3::blank(),
	//	custom::Vector3(300.0,0.0,0.0),
	//	0.3,
	//	0.1,
	//	{0.5,0.5,0.5,0.5},
	//	0.0
	//);
	//phys_gen_config exhgen(config.position, 1, UNIFORM, 200.0);
	//firing_system->add_gen(new TimedPhysicsParticleGenerator(scene,exhgen, exhaust, 0.1));


	blast_system = new PhysicsParticleSystem(s);
	phys_particle_config proj2(
		custom::Vector3::blank(),
		custom::Vector3::blank(),
		1.0,
		0.1,
		{0,0,1,1},
		3,
		true
	);
	phys_gen_config gen2(config.position, 200,
		distribution::UNIFORM);
	blast_system->add_gen(new PhysicsParticleGenerator(scene,gen2, proj2));

	//bullet_drag = new PhysicsWindGen(s,custom::Vector3::convert(pose.p), custom::Vector3(10, 0, 0));
	//firing_system->add_force(bullet_drag);
	//drag = new PhysicsWindGen(custom::Vector3::convert(pose.p), custom::Vector3::blank(), 0.5);
	//max_speed = 200.0;
}

physics::PlayerShip::~PlayerShip()
{
	delete firing_system;
	delete blast_system;
	delete drag;
}

void physics::PlayerShip::step(double dt)
{
	firing_system->step(dt);
	blast_system->step(dt);
	//std::cout << vel.getX() << " " << vel.getY() << " " << vel.getZ() << "\n";
	drag->applyForce(this);
	if (custom::Vector3::convert(dynActor->getLinearVelocity()).mod() < stop_threshold) {
		dynActor->setLinearVelocity(custom::Vector3::blank().converted());
		dynActor->clearForce();
		//vel = custom::Vector3::blank();
		//accel = custom::Vector3::blank();
	}
	//drag->redirect(vel * -1);
	//std::cout << vel.mod() << "\n";
	DynamicPhysicsObject::step(dt);
}

void physics::PlayerShip::translate(custom::Vector3 v)
{
	DynamicPhysicsObject::translate(v);
	firing_system->translate(v);
	blast_system->translate(v);
	drag->translate(v);
}
