#include <windows.h>
#include <gl\glut.h>
#include <iostream>
#include <vector>
#include "Point.h"
using namespace std;

bool finish = false;
vector<Point> input;
int wheigth = 600;

Point Bezier(Point p0, Point p1, Point p2, double t)
{
	double t1 = 1 - t;
	return (t1*t1*p0 + 2*t*t1*p1 + t*t*p2);
}

void computeBezier()
{
	vector<Point> output;
	for (int i = 0; i < input.size() - 2; i+=3)
	{
		for (double t = 0; t <= 1; t+=0.05)
			output.push_back(Bezier(input[i], input[i + 1], input[i + 2], t));
	}
	input = output;
}

void saveMap()
{
	for (int i = 0; i < input.size(); i++)
		cout << input[i].x << " " << input[i].y << " ";
}

void AppRender()
{
	glClear ( GL_COLOR_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1, 0, 0);
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < input.size(); i++)
		glVertex2d(input[i].x, input[i].y);
	glEnd();
	if (input.size() > 1)
	{
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		for (int i = 0; i < input.size() - 1; i++)
		{
			glVertex2d(input[i].x, input[i].y);
			glVertex2d(input[i + 1].x, input[i + 1].y);
		}
		glEnd();
	}
	glutSwapBuffers();
}

void AppReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	wheigth = height;
}

void AppKeyboardUp(unsigned char key, int x, int y)
{
	if (key == 27) finish = true;
	else if (key == 98) computeBezier();
	else if (key == 115) saveMap();
}

void AppMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		input.push_back(Point(x, wheigth-y));
}

void AppIdle()
{
	if(finish) exit(0);
	else AppRender();
}

void main(int argc, char** argv)
{
	int res_x,res_y,pos_x,pos_y;

	cerr << "Usage:" << endl;
	cerr << "\t 1. Make a line like a saw in horizontal direction" << endl;
	cerr << "\t 2. Define only maximums and minimums with three points" << endl;
	cerr << "\t 3. Press B to make the curves" << endl;
	cerr << "\t 4. Press S to save file" << endl;
	cerr << "\t 5. File has been created on Map creator path with name map.txt" << endl;
	cerr << "\t 6. Update map coordinates in one of level file with map.txt content" << endl;
	cerr << "\t 7. You have created your own map!" << endl << endl;

	// GLUT initialization
	glutInit(&argc, argv);

	// RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	// Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(800>>1);
	pos_y = (res_y>>1)-(600>>1);
	
	glutInitWindowPosition(pos_x, pos_y);
	glutInitWindowSize(800,600);
	glutCreateWindow("DownhillRacing Map Creator!");

	/*glutGameModeString("800x600:32");
	glutEnterGameMode();*/

	// Make the default cursor disappear
	//glutSetCursor(GLUT_CURSOR_NONE);

	// Register callback functions
	glutDisplayFunc(AppRender);
	glutReshapeFunc(AppReshape);
	glutKeyboardUpFunc(AppKeyboardUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);

	// Application loop
	glutMainLoop();	
}
