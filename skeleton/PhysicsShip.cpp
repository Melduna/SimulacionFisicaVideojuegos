#include "PhysicsShip.h"

physics::PhysicsShip::PhysicsShip(physx::PxScene* s, phys_particle_config config, double sf, double df, double r):
	SphereParticle(s, config,sf,df,r)
{
	dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

}

physics::PlayerShip::PlayerShip(physx::PxScene* s, phys_particle_config config):
	PhysicsShip(s,config,5000.0,5000.0,0.0001)
{
	dynActor->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	firing_system = new PhysicsParticleSystem(scene);
	phys_particle_config proj1(
		custom::Vector3(-100,0,0),
		custom::Vector3(-1000,0,0),
		1.0,
		10.0,
		{ 1,0,0,1 },
		1.0,
		true
	);
	phys_gen_config gen1(config.position, 1,
		distribution::UNIFORM,600);
	firing_system->add_gen(new PhysicsParticleGenerator(scene,gen1, proj1)); //Primary fire
	
	phys_particle_config crosshair_config(config.position + custom::Vector3(-200, 0, 0), custom::Vector3::blank(), 10.0, 10, { 0,0,0,1 });
	crosshair = new Crosshair(s,crosshair_config);
	drag = new PhysicsWindGen(s,config.position, custom::Vector3::blank(), 0.01);
	max_speed = 600.0;
	spring = new PhysicsSpringGen(s, 20.0, 200, this);
}

physics::PlayerShip::~PlayerShip()
{
	delete firing_system;
	delete crosshair;
	delete spring;
	//delete blast_system;
	delete drag;
}

void physics::PlayerShip::step(double dt)
{
	firing_system->step(dt);
	//blast_system->step(dt);
	//std::cout << vel.getX() << " " << vel.getY() << " " << vel.getZ() << "\n";
	drag->applyForce(this);
	if (crosshair) crosshair->step(dt);
	auto vec = custom::Vector3::convert(dynActor->getLinearVelocity());
	if (vec.mod() < stop_threshold) {
		//std::cout << vec.mod() << "\n";
		dynActor->setLinearVelocity(custom::Vector3::blank().converted());
		dynActor->clearForce();
		//vel = custom::Vector3::blank();
		//accel = custom::Vector3::blank();
	}
	//drag->redirect(vel * -1);
	//std::cout << vel.mod() << "\n";

	DynamicPhysicsObject::step(dt);
	drag->translate(delta);
	firing_system->translate(delta);
	if (crosshair)crosshair->translate(delta);
	if (crosshair)spring->applyForce(crosshair);
}

void physics::PlayerShip::translate(custom::Vector3 v)
{
	DynamicPhysicsObject::translate(v);
	firing_system->translate(v);
	//blast_system->translate(v);
	drag->translate(v);
}

void physics::PlayerShip::fire()
{
	crosshair->addForce(custom::Vector3(-500, 0, 0));
	firing_system->fire_at(0);
}

void physics::PlayerShip::reset()
{
	physics::DynamicPhysicsObject::reset();
	//crosshair->resetMovement();
	//crosshair->setPosition(init_pos+ custom::Vector3(-200, 0, 0));
	//drag->reset();
	if (crosshair) {
		delete crosshair; crosshair = nullptr;
	}
	phys_particle_config crosshair_config(init_pos + custom::Vector3(-200, 0, 0), custom::Vector3::blank(), 10.0, 10, { 0,0,0,1 });
	crosshair = new Crosshair(scene,crosshair_config);
	spring->reset();
	//firing_system->reset();
}

void physics::PlayerShip::reset_stage_1()
{
	physics::DynamicPhysicsObject::reset();
	if (crosshair) {
		delete crosshair; crosshair = nullptr;
	}
	spring->reset();
}

void physics::PlayerShip::reset_stage_2()
{
	if (crosshair) {
		delete crosshair; crosshair = nullptr;
	}
	phys_particle_config crosshair_config(init_pos + custom::Vector3(-200, 0, 0), custom::Vector3::blank(), 10.0, 10, { 0,0,0,1 });
	crosshair = new Crosshair(scene, crosshair_config);
}

physics::EnemyShip::EnemyShip(physx::PxScene* s, phys_particle_config config, int i):PhysicsShip(s,config),index(i)
{
	blast = new PhysicsParticleSystem(scene);
	phys_particle_config shrapnel(custom::Vector3::blank(),custom::Vector3::blank(),1.0,5.0,{1,0,0,1});
	phys_gen_config blastgen(config.position, 100, UNIFORM, 100,true);
	blast->add_gen(new PhysicsParticleGenerator(s, blastgen, shrapnel));
	dynActor->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z |
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y);

	char name[6];
	sprintf(name,"ship_%d", i);
	dynActor->setName(name);
}

physics::EnemyShip::~EnemyShip()
{
	delete blast;
}

void physics::EnemyShip::step(double dt)
{
	DynamicPhysicsObject::step(dt);
	blast->step(dt);
}

bool physics::EnemyShip::die()
{
	if (alive) {
		setOpacity(0);
		blast->fire_at(0);
		blast->add_force(new PhysicsExplosionGen(scene, custom::Vector3::convert(dynActor->getGlobalPose().p), 500.0));
		alive = false;
		return true;
	}
	return false;
}
