#pragma once

#include "Point.h"

class Point;

class Vector
{
public:
	float x;
	float y;
	float z;

public:
	Vector(float vx = 0.0, float vy = 0.0, float vz = 0.0);
	Vector(const Point& p);
	~Vector(void);
	float length(void) const;
	float squaredLength(void) const;
	void normalize(void);
};

Vector operator+(const Vector& v1, const Vector& v2);
Vector& operator+=(Vector& v1, const Vector& v2);
Vector operator-(const Vector& v1, const Vector& v2);
Vector& operator-=(Vector& v1, const Vector& v2);
Vector operator*(const Vector& v, float f);
Vector operator*(float f, const Vector& v);
Vector operator*=(Vector& v, float f);
float operator*(const Vector& v1, const Vector& v2);
Vector operator/(const Vector& v, float f);
Vector& operator/=(Vector& v, float f);
Vector crossProduct(const Vector& v1, const Vector& v2);
