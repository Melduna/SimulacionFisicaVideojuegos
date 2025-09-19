#pragma once
#include <cmath>
class Vector3 {
private:
	float x;
	float y;
	float z;
public:
	Vector3(int x_, int y_, int z_) :x(x_), y(y_), z(z_) {}
	float getX() const { return x; }
	float getY() const { return y; }
	float getZ() const { return z; }
	Vector3 operator+(const Vector3& other) {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator*(float n) {
		return Vector3(x * n, y * n, z * n);
	}
	Vector3 operator-(Vector3& other) {
		return *this + (other * (-1.0f));
	}
	Vector3 operator=(Vector3& other) {
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
	static Vector3 normalized(Vector3& vec) {
		return (vec * (1.0f / vec.mod()));
	}
};