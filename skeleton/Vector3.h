#pragma once
#include <cmath>
#include <iostream>
#include "PxActor.h"
namespace custom {
	class Vector3 {
	private:
		float x;
		float y;
		float z;
	public:
		Vector3() :x(0.0), y(0.0), z(0.0) {}
		Vector3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
		inline float getX() const { return x; }
		inline float getY() const { return y; }
		inline float getZ() const { return z; }
		Vector3 operator+(const Vector3& other) {
			return Vector3(x + other.x, y + other.y, z + other.z);
		}
		void operator+=(const Vector3& other) {
			x += other.x, y += other.y, z += other.z;
		}
		Vector3 operator*(float n) {
			return Vector3(x * n, y * n, z * n);
		}
		void operator*=(float n) {
			x *= n, y *= n, z *= n;
		}
		Vector3 operator-(Vector3& other) {
			return *this + (other * (-1.0f));
		}
		void operator-=(Vector3& other) {
			*this += (other * (-1.0f));
		}
		void operator=(Vector3& other) {
			this->x = other.x; this->y = other.y; this->z = other.z;
		}
		static float dot(const Vector3& a, const Vector3& b) {
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}
		float mod() {
			return sqrt(x * x + y * y + z * z);
		}
		void normalize() {
			*this = (*this * (1.0f / this->mod()));
		}
		Vector3 normalized() {
			if (this->mod() < 0.0001) return *this;
			return (*this * (1.0f / mod()));
		}
		physx::PxVec3 converted() {
			return physx::PxVec3(x, y, z);
		}
		static Vector3 convert(physx::PxVec3 v) {
			return Vector3(v.x, v.y, v.z);
		}
		void print() {
			std::cout << x << " " << y << " " << z << " " << "\n";
		}
		static Vector3 blank() {
			return Vector3(0, 0, 0);
		}
	};
}