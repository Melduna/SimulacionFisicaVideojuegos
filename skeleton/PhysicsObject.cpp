#include "PhysicsObject.h"
physics::PhysicsObject::PhysicsObject(physx::PxScene* s) :scene(s) {
	auto gphysics = &PxGetPhysics();
	mat = gphysics->createMaterial(1.0f, 1.0f, 1.0f);
}

physics::PhysicsObject::~PhysicsObject() 
{
	if (_renderItem)
		DeregisterRenderItem(_renderItem);
	for (auto c : children) if (c) delete c;
}

void physics::PhysicsObject::step(double dt)
{
	if (timed) {
		lifetime -= dt;
		if (dt <= 0.0) {
			alive = false;
		}
	}
	if (actor) {
		new_pos = custom::Vector3::convert(actor->getGlobalPose().p);
		for (auto c : children) if (c) c->translate(new_pos - last_pos);
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

custom::Vector3 physics::PhysicsObject::getPosition()
{
	if (actor) return custom::Vector3::convert(actor->getGlobalPose().p);
	return custom::Vector3::blank();
}

physics::DynamicPhysicsObject::DynamicPhysicsObject(physx::PxScene* s, phys_particle_config config) :
	PhysicsObject(s) 
{
	auto gphysics = &PxGetPhysics();
	dynActor = gphysics->createRigidDynamic(physx::PxTransform(config.position.converted(), physx::PxQuat(physx::PxIdentity)));
	actor = dynActor;
	scene->addActor(*dynActor);
	timed = config.timed;
}
custom::Vector3 physics::DynamicPhysicsObject::getDirection()
{
	return custom::Vector3::convert(dynActor->getLinearVelocity());
	return custom::Vector3();
}

void physics::DynamicPhysicsObject::addForce(custom::Vector3 f)
{
	dynActor->addForce(f.converted());
}

physics::StaticPhysicsObject::StaticPhysicsObject(physx::PxScene* s, phys_particle_config config) :
	PhysicsObject(s) 
{
	auto gphysics = &PxGetPhysics();
	stcActor = gphysics->createRigidStatic(physx::PxTransform(config.position.converted(), physx::PxQuat(physx::PxIdentity)));
	stcActor->getGlobalPose();
	actor = stcActor;
	scene->addActor(*stcActor);
}

physics::SphereParticle::SphereParticle(physx::PxScene* s, phys_particle_config config) :
	DynamicPhysicsObject(s, config) 
{
	auto gphysics = &PxGetPhysics();
	auto sphere = gphysics->createShape(physx::PxSphereGeometry(config.size), *mat);
	_renderItem = new RenderItem(sphere, dynActor, { 1,1,1,1 });
	dynActor->attachShape(*sphere);
	dynActor->setMass(config.mass);
	physx::PxRigidBodyExt::updateMassAndInertia(*dynActor, config.mass / (4 * pow(config.size, 3) * PI / 3));
	dynActor->setLinearVelocity(config.velocity.converted());
}