#include <windows.h>
#include <gl/glut.h>

#include "Scene.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

Scene scene;

void init(void)
{
	glClearColor(0.35f, 0.35f, 0.6f, 1.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING); 
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void render(void)   
{
	scene.render();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
	float radius;
	Point center;
	scene.boundingSphere(center, radius);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width/(float)height, radius, 3*radius);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-2*radius);
	glRotatef(-0,0,0,1);
	glRotatef(0,1,0,0);
	glRotatef(-0,0,1,0);
	glTranslatef(-center.x, -center.y, -center.z);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:	exit(0);
					break;
	}
}

void special_keys(int a_keys, int x, int y)
{
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Downhill Racing");
	/*
	glutGameModeString("640x480:16");
	glutEnterGameMode();
	*/
	glutSetCursor(GLUT_CURSOR_NONE);

	init();
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keys);
	glutMainLoop();
}