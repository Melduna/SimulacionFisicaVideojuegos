#include "PhysicsObject.h"
void physics::PhysicsObject::step(double dt)
{
	if (timed) {
		lifetime -= dt;
		if (dt <= 0.0) {
			alive = false;
		}
	}
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

custom::Vector3 physics::DynamicPhysicsObject::getDirection()
{
	return custom::Vector3::convert(dynActor->getLinearVelocity());
	return custom::Vector3();
}

void physics::DynamicPhysicsObject::addForce(custom::Vector3 f)
{
	dynActor->addForce(f.converted());
}
