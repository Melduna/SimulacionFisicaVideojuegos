#include "GameObject.h"

void GameObject::step(double t)
{
	if (timed) {
		if (lifetime <= 0.0) alive = false;
		else lifetime -= t;
	}
	for (auto p : children) if (p) p->step(t);
}

void GameObject::translate(custom::Vector3 t)
{
	pose.p += t.converted();
	for (auto c : children) c->translate(t);
}

Particle::Particle(particle_config c)
{
	timed = true;
	pose = physx::PxTransform(c.position.converted());
	_renderItem = new Sphere(&pose,c.size,c.color);
	vel = c.velocity;
	accel = custom::Vector3::blank();
	lifetime = c.lifetime;
	mass_simulated = c.mass;
	speed_simulated = c.velocity.mod();
	mass_inverse = 1.0 / c.mass;
	gravity_simulated = c.gravity;
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
	custom::Vector3 trueAccel = accel + custom::Vector3(0.0,-gravity_simulated,0.0);
	vel += trueAccel * t;
	vel *= damping;
	if (max_speed >= 0 && max_speed < vel.mod()) {
		vel.normalize();
		vel *= max_speed*0.9;
		accel = custom::Vector3::blank();
	}
	translate(vel);
	force_accum = custom::Vector3::blank();
}

void Particle::step(double t)
{
	integrate(t);
	GameObject::step(t);
}

Projectile::Projectile(projectile_config c) :
	Particle(c.p_config), gravity_real(c.p_config.gravity)
{
	mass_real = c.p_config.mass;
	speed_real = c.speed;
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
	mass_simulated = mass_real * speed_factor();
	if (mass_simulated == 0) mass_inverse = 1.0/mass_real;
	else mass_inverse = 1.0 / mass_simulated;
}

double Projectile::speed_factor()
{
	return (speed_simulated * speed_simulated) / (speed_real * speed_real);
}




