#include "Vector.h"

#include <cmath>

Vector::Vector(float vx, float vy, float vz):x(vx),y(vy),z(vz) {}

Vector::Vector(const Point& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

Vector::~Vector(void) {}

Vector operator+(const Vector& v1, const Vector& v2)
{
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector& operator+=(Vector& v1, const Vector& v2)
{
	v1 = v1 + v2;

	return v1;
}

Vector operator-(const Vector& v1, const Vector& v2)
{
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector& operator-=(Vector& v1, const Vector& v2)
{
	v1 = v1 - v2;

	return v1;
}

Vector operator*(const Vector& v, float f)
{
	return Vector(v.x*f, v.y*f, v.z*f);
}

Vector operator*(float f, const Vector& v)
{
	return v*f;
}

Vector operator*=(Vector& v, float f)
{
	v.x*=f;
	v.y*=f;
	v.z*=f;

	return v;
}

float operator*(const Vector& v1, const Vector& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vector operator/(const Vector& v, float f)
{
	float inv = 1.0/f;

	return v*inv;
}

Vector& operator/=( Vector& v, float f )
{
	float inv = 1.0/f;

	v.x*=inv;
	v.y*=inv;
	v.z*=inv;

	return v;
}

Vector crossProduct(const Vector& v1, const Vector& v2)
{
	Vector res;

	res.x = (v1.y * v2.z) - (v1.z * v2.y);
	res.y = (v1.z * v2.x) - (v1.x * v2.z);
	res.z = (v1.x * v2.y) - (v1.y * v2.x);

	return res;
}

float Vector::length(void) const
{
	return sqrt(squaredLength());
}

float Vector::squaredLength(void) const
{
	return x*x + y*y + z*z;	
}

void Vector::normalize(void)
{
	float inv = 1.0/length();

	x*=inv;
	y*=inv;
	z*=inv;
}