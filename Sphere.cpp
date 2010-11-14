#include "Sphere.h"
#include <gl/glut.h>

Sphere::Sphere(Point c, float r) : center(c), radius(r)
{}


Sphere::~Sphere(void)
{}

void Sphere::render() {
	glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		GLUquadricObj *quad;
		quad = gluNewQuadric();
		gluSphere(quad, radius/2, DEPTH, DEPTH);
	glPopMatrix();
}