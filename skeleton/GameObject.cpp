#include "GameObject.h"

void GameObject::step(double t)
{
	if (lifetime <= 0.0) alive = false;
	else lifetime -= t;
}

void GameObject::translate(custom::Vector3 &t)
{
	pose.p += t.converted();
	for (auto c : children) c->translate(t);
}

Particle::Particle(particle_config c)
{
	pose = physx::PxTransform(c.position.converted());
	_renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(10)), &pose, c.color);
	vel = c.velocity;
	accel = custom::Vector3(0.0, 0.0, 0.0);
	lifetime = c.lifetime;
	mass_inverse = 1.0 / mass_simulated;
}

Particle::~Particle()
{
	//renderItem->release();
	if (_renderItem != nullptr)
		DeregisterRenderItem(_renderItem);

	for (auto p : children) if (p) delete p;
	
}

void Particle::integrate(double t)
{
	accel += force_accum * mass_inverse;
	vel += accel * t;
	vel *= damping;
	translate(vel);
	force_accum = custom::Vector3(0, 0, 0);
}

void Particle::step(double t)
{
	integrate(t);
	GameObject::step(t);
	for (auto p : children) if (p) p->step(t);
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
	//vs2/vr2
	gravity_simulated = gravity_real * speed_factor();
}

void Projectile::update_mass_s()
{
	mass_simulated = mass_simulated * speed_factor();
	mass_inverse = 1.0 / mass_simulated;
}

double Projectile::speed_factor()
{
	return (speed_simulated * speed_simulated) / (speed_real * speed_real);
}

void Projectile::integrate(double t)
{
	Particle::integrate(t);
}


