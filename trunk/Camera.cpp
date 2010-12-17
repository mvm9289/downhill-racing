#include "Camera.h"

#include <gl/glut.h>

Camera::Camera(void)
{
	type = PERSPECTIVE;
	sceneCenter = Point();
	pos = Point();
	falsePos = Point();
	VRP = Point();
	up = Vector();
	fovy = 0.0;
	aspectRatio = 0.0;
	zNear = 0.0;
	zFar = 0.0;
	left = 0.0;
	right = 0.0;
	bottom = 0.0;
	top = 0.0;
	rotX = 0.0;
	rotY = 0.0;
}

Camera::Camera(Point c, Point p, Point vrp, Vector u, double f, double ar, double zN, double zF)
{
	type = PERSPECTIVE;

	sceneCenter = c;
	pos = p;
	falsePos = p;
	falsePos.y = vrp.y;
	VRP = vrp;
	up = u;

	fovy = f;
	aspectRatio = ar;
	zNear = zN;
	zFar = zF;

	left = 0.0;
	right = 0.0;
	bottom = 0.0;
	top = 0.0;

	rotX = 0.0;
	rotY = 0.0;
}

Camera::Camera(Point p, Point vrp, Vector u, double l, double r, double b, double t, double zN, double zF)
{
	type = ORTHO;

	pos = p;
	falsePos = p;
	falsePos.y = vrp.y;
	VRP = vrp;
	up = u;

	left = l;
	right = r;
	bottom = b;
	top = t;
	zNear = zN;
	zFar = zF;

	sceneCenter = Point();
	fovy = 0.0;
	aspectRatio = 0.0;

	rotX = 0.0;
	rotY = 0.0;
}

Camera::~Camera(void)
{
}

void Camera::init()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z, VRP.x, VRP.y, VRP.z, up.x, up.y, up.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (type)
	{
		case ORTHO:
			glOrtho(left, right, bottom, top, zNear, zFar);
			break;
		case PERSPECTIVE:
		default:
			gluPerspective(fovy, aspectRatio, zNear, zFar);
			break;
	}
}

void Camera::reshape(double ar)
{
	aspectRatio = ar;
	init();
}

void Camera::rotate(double x, double y)
{
	float m[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);
	glMultMatrixf(&m[0][0]);
}

void Camera::move(double x, double z)
{
	float m[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-x, 0, -z);
	glMultMatrixf(&m[0][0]);
}

void Camera::move(Point p)
{
	Vector dir = (p + Vector(0, 2, 0)) - VRP;
	Vector dirNorm = dir;
	dirNorm.normalize();
	Vector moveDir = VRP - falsePos;
	moveDir.normalize();
	VRP += dir;
	pos += dir;
	falsePos += dir;
	pos.y += (moveDir.y - dirNorm.y);
	falsePos.y += (moveDir.y - dirNorm.y);
	init();
}

Vector Camera::getVisionDir() {
	Vector visio = VRP - pos;
	visio.normalize();
	return visio;
}