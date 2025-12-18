#include "PhysicsObject.h"
physics::PhysicsObject::PhysicsObject(physx::PxScene* s,double sf, double df, double r) :scene(s) {
	auto gphysics = &PxGetPhysics();
	mat = gphysics->createMaterial(sf, df, r);
}

physics::PhysicsObject::~PhysicsObject() 
{
	if (_renderItem)
		DeregisterRenderItem(_renderItem);
	if (actor)
		actor->release();
	for (auto c : children) if (c) delete c;
}

void physics::PhysicsObject::step(double dt)
{
	if (timed) {
		lifetime -= dt;
		if (lifetime <= 0.0) {
			alive = false;
		}
	}
	if (actor) {
		new_pos = custom::Vector3::convert(actor->getGlobalPose().p);
		delta = new_pos - last_pos;
		for (auto c : children) if (c) c->translate(delta);
		last_pos = custom::Vector3::convert(actor->getGlobalPose().p);
	}
}

void physics::PhysicsObject::translate(custom::Vector3 v)
{
	if (actor) actor->setGlobalPose(physx::PxTransform(actor->getGlobalPose().p + v.converted()));
	for (auto c : children) if (c) c->translate(new_pos - last_pos);
}

void physics::PhysicsObject::setParent(PhysicsObject* p)
{
	parent = p;
}

void physics::PhysicsObject::addChild(PhysicsObject* child)
{
	children.push_back(child);
	child->setParent(this);
}

void physics::PhysicsObject::setOpacity(double a)
{
	if (_renderItem) _renderItem->color.w = a;
	
}

custom::Vector3 physics::PhysicsObject::getPosition()
{
	if (actor) return custom::Vector3::convert(actor->getGlobalPose().p);
	return custom::Vector3::blank();
}

void physics::PhysicsObject::setPosition(custom::Vector3 v)
{
	actor->setGlobalPose(physx::PxTransform(v.converted()));
}

physics::DynamicPhysicsObject::DynamicPhysicsObject(physx::PxScene* s, phys_particle_config config, double sf, double df, double r) :
	PhysicsObject(s,sf,df,r) 
{

	init_pos = config.position;

	last_pos = config.position;
	auto gphysics = &PxGetPhysics();
	dynActor = gphysics->createRigidDynamic(physx::PxTransform(config.position.converted(), physx::PxQuat(physx::PxIdentity)));
	actor = dynActor;
	scene->addActor(*dynActor);
	if (!config.gravity)
		dynActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	timed = config.timed;
	lifetime = config.lifetime;
}
custom::Vector3 physics::DynamicPhysicsObject::getDirection()
{
	return custom::Vector3::convert(dynActor->getLinearVelocity());
}

void physics::DynamicPhysicsObject::addForce(custom::Vector3 f)
{
	dynActor->addForce(f.converted(),physx::PxForceMode::eIMPULSE);
}

void physics::DynamicPhysicsObject::step(double dt)
{
	PhysicsObject::step(dt);
	custom::Vector3 vel = custom::Vector3::convert(dynActor->getLinearVelocity());
	if (max_speed >= 0 && max_speed < vel.mod()) {
		vel.normalize();
		vel *= max_speed * 0.9;
		dynActor->setLinearVelocity(vel.converted());
		dynActor->clearForce();
	}
}

void physics::DynamicPhysicsObject::reset()
{
	actor->setGlobalPose(physx::PxTransform(init_pos.converted()));
	dynActor->clearForce();
	dynActor->setLinearVelocity(Vector3(0, 0, 0));
}

void physics::DynamicPhysicsObject::resetMovement()
{
	dynActor->clearForce();
	dynActor->setLinearVelocity(Vector3(0, 0, 0));
}

physics::StaticPhysicsObject::StaticPhysicsObject(physx::PxScene* s, phys_particle_config config,double sf, double df, double r) :
	PhysicsObject(s,sf,df,r) 
{
	last_pos = config.position;
	auto gphysics = &PxGetPhysics();
	stcActor = gphysics->createRigidStatic(physx::PxTransform(config.position.converted(), physx::PxQuat(physx::PxIdentity)));
	stcActor->getGlobalPose();
	actor = stcActor;
	scene->addActor(*stcActor);
}

physics::SphereParticle::SphereParticle(physx::PxScene* s, phys_particle_config config, double sf, double df, double r) :
	DynamicPhysicsObject(s, config,sf,df,r) 
{
	auto gphysics = &PxGetPhysics();
	auto sphere = gphysics->createShape(physx::PxSphereGeometry(config.size), *mat);
	_renderItem = new RenderItem(sphere, dynActor, config.color);
	dynActor->attachShape(*sphere);
	dynActor->setMass(config.mass);
	physx::PxRigidBodyExt::updateMassAndInertia(*dynActor, config.mass / (4 * pow(config.size, 3) * PI / 3));
	dynActor->setLinearVelocity(config.velocity.converted());
}

physics::Crosshair::Crosshair(physx::PxScene* s, phys_particle_config config):DynamicPhysicsObject(s,config)
{
	auto gphysics = &PxGetPhysics();
	double halfsize = config.size / 2;
	auto cube = gphysics->createShape(physx::PxBoxGeometry(Vector3(halfsize, halfsize, halfsize)), *mat);
	_renderItem = new RenderItem(cube, dynActor, config.color);
	dynActor->setMass(config.mass);
	physx::PxRigidBodyExt::updateMassAndInertia(*dynActor, config.mass/config.size);
	dynActor->setLinearVelocity(config.velocity.converted());
	dynActor->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z|
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y|
		physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z);
}

physics::Wall::Wall(physx::PxScene* s, phys_particle_config config,bool vert):StaticPhysicsObject(s,config,50000.0,50000.0,1.0)
{
	auto gphysics = &PxGetPhysics();
	auto box = gphysics->createShape(physx::PxBoxGeometry(Vector3(500, vert ? config.size : 10, vert ? 10 : config.size)), *mat);
	stcActor->attachShape(*box);
	//_renderItem = new RenderItem(box, stcActor, config.color);
}

physics::GhostSphereParticle::GhostSphereParticle(physx::PxScene* s, phys_particle_config config):DynamicPhysicsObject(s,config)
{
	auto gphysics = &PxGetPhysics();
	auto sphere = gphysics->createShape(physx::PxSphereGeometry(config.size), *mat);
	_renderItem = new RenderItem(sphere, dynActor, config.color);
	dynActor->setMass(config.mass);
	physx::PxRigidBodyExt::updateMassAndInertia(*dynActor, config.mass / (4 * pow(config.size, 3) * PI / 3));
	dynActor->setLinearVelocity(config.velocity.converted());
}
