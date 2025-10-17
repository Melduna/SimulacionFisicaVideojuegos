#include "PhysicsObjects.h"

Particle::Particle(particle_config c)
{
	pose = physx::PxTransform(c.position.converted());
	_renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(10)), &pose, Vector4(1, 1, 1, 1));
	vel = c.velocity;
	accel = custom::Vector3(0.0, 0.0, 0.0);
	lifetime = c.lifetime;
	remaining_lifetime = lifetime;
}

Particle::~Particle()
{
	//renderItem->release();
	if (_renderItem != nullptr)
		DeregisterRenderItem(_renderItem);
}

void Particle::integrate(double t)
{
	vel += accel * t;
	vel *= damping;
	pose.p += vel.converted();
	remaining_lifetime -= t;
	if (remaining_lifetime <= 0.0) {
		alive = false;
		DeregisterRenderItem(_renderItem);
		_renderItem = nullptr;
	}
}

Projectile::Projectile(projectile_config c) :
	Particle(c.p_config), mass_real(c.mass), speed_real(c.speed), gravity_real(c.gravity), speed_simulated(c.p_config.velocity.mod())
{
	update_mass_s();
	update_gravity_s();
	set_accel(custom::Vector3(0, -gravity_simulated, 0));
}

void Projectile::update_gravity_s()
{
	gravity_simulated = gravity_real * speed_factor();
}

void Projectile::update_mass_s()
{
	mass_simulated = mass_simulated * speed_factor();
}

double Projectile::speed_factor()
{
	return (speed_simulated * speed_simulated) / (speed_real * speed_real);
}

//void Projectile::integrate(double t)
//{
//	custom::Vector3 trueAccel = gravAccel + accel;
//	vel += trueAccel * t;
//	vel *= damping;
//	pose.p += vel.converted();
//}