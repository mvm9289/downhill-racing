#include "Point.h"

Point::Point(float px, float py, float pz):x(px),y(py),z(pz) {}

Point::~Point(void)
{
}

Point operator+(const Point& p1, const Point& p2)
{
	return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Point operator+(const Point& p, const Vector& v)
{
	return Point(p.x + v.x, p.y + v.y, p.z + v.z);
}

Point& operator+=(Point& p1, const Point& p2)
{
	p1.x+=p2.x;
	p1.y+=p2.y;
	p1.z+=p2.z;

	return p1;
}

Point& operator+=(Point& p, const Vector& v)
{
	p.x+=v.x;
	p.y+=v.y;
	p.z+=v.z;

	return p;
}

Vector operator-(const Point& p1, const Point& p2)
{
	return Vector(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Point operator-(const Point& p, const Vector& v)
{
	return Point(p.x - v.x, p.y - v.y, p.z - v.z);
}

Point& operator-=(Point& p, const Vector& v)
{
	p.x-=v.x;
	p.y-=v.y;
	p.z-=v.z;

	return p;
}

Point operator*(const Point& p, float f)
{
	return Point(p.x*f, p.y*f, p.z*f);
}

Point operator*(float f, const Point& p)
{
	return p*f;
}

Point operator/(const Point& p, float f)
{
	float inv = 1.0/f;

	return p*inv;
}

Point& operator/=(Point& p, float f)
{
	float inv = 1.0/f;

	p.x*=inv;
	p.y*=inv;
	p.z*=inv;

	return p;
}