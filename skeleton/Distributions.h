#pragma once
#include <random>
class Distributions {
public:
	Distributions() {
		unif = std::uniform_real_distribution<double>(0.0, 1.0);
		norm = std::normal_distribution<double>(0.0, 1.0);
		std::random_device rd;
		seed = std::mt19937(rd());
	};
	static double next_uniform(double range = 1.0)
	{
		double output = unif(seed)*range;
		//std::cout << "uniform: " << output << "\n";
		return output;
	};
	static double next_normal(double range = 1.0)
	{
		double output = norm(seed)*range;
		//std::cout << "normal: " << output << "\n";
		return output;
	};
protected:
	inline static std::uniform_real_distribution<double> unif;
	inline static std::normal_distribution<double> norm;
	inline static std::mt19937 seed;
};
