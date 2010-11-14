#pragma once

#include "Vector.h"

class Vector;

class Point
{
public:
	float x;
	float y;
	float z;

public:
	Point(float px = 0.0, float py = 0.0, float pz = 0.0);
	~Point(void);
};

Point operator+(const Point& p1, const Point& p2);
Point operator+(const Point& p, const Vector& v);
Point& operator+=(Point& p1, const Point& p2);
Point& operator+=(Point& p, const Vector& v);
Vector operator-(const Point& p1, const Point& p2);
Point operator-(const Point& p, const Vector& v);
Point& operator-=(Point& p, const Vector& v);
Point operator*(const Point& p, float f);
Point operator*(float f, const Point& p);
Point operator/(const Point& p, float f);
Point& operator/=(Point& p, float f);
