#pragma once
#include <random>
class Distributions {
public:
	Distributions() {
		unif = std::uniform_real_distribution<double>(0.0, 1.0);
		norm = std::normal_distribution<double>(0.0, 1.0);
		ample_norm = std::normal_distribution<double>(0.0, 50.0);
		std::random_device rd;
		seed = std::mt19937(rd());
	};
	static double next_uniform()
	{
		return unif(seed);
	};
	static double next_normal()
	{
		return norm(seed);
	};
	static double next_ample_normal()
	{
		return ample_norm(seed);
	};
protected:
	inline static std::uniform_real_distribution<double> unif;
	inline static std::normal_distribution<double> norm;
	inline static std::normal_distribution<double> ample_norm;
	inline static std::mt19937 seed;
};
