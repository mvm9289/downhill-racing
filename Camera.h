#pragma once

#include "Point.h"
#include "Vector.h"

#define PERSPECTIVE 1
#define ORTHO 2

class Camera
{
private:
	int type;
	Point sceneCenter;
	Point pos;
	Point VRP;
	Vector up;
	double fovy;
	double aspectRatio;
	double zNear;
	double zFar;
	double left;
	double right;
	double bottom;
	double top;
	double rotX;
	double rotY;
public:
	Camera(void);
	Camera(Point c, Point p, Point vrp, Vector u, double f, double ar, double zN, double zF);
	Camera(Point p, Point vrp, Vector u, double l, double r, double b, double t, double zN, double zF);
	~Camera(void);
	void init();
	void reshape(double ar);
	void rotate(double x, double y);
	void move(double x, double z);
	void move(Point p);
};
