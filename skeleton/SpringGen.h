#pragma once
#include "ForceGenerator.h"
#include "Vector3.h"
class SpringGen : public ForceGenerator {
public:
	SpringGen(double k, double resting_length, Particle* other);

	virtual void apply_force(Particle*) override;

	inline void set_k(double k) { _k = k; }
protected:
	double _k;
	double _resting_length;
	Particle* _other;
};
class AnchoredSpringGen : public SpringGen {
public:
	AnchoredSpringGen(double k, double resting_length, custom::Vector3 anchor_pos);
	~AnchoredSpringGen();
};