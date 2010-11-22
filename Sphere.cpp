#include "Sphere.h"
#include <gl/glut.h>

Sphere::Sphere(Point c, float r) : center(c), radius(r), textID(-1)
{}


Sphere::~Sphere(void)
{}

void Sphere::render() {
	GLUquadricObj *quad;
	quad = gluNewQuadric();

	if (textID > 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textID);
		gluQuadricTexture(quad, GL_TRUE);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	}

	glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		gluSphere(quad, radius, DEPTH, DEPTH);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void Sphere::setTextureID(GLuint id) {
	textID = id;
}