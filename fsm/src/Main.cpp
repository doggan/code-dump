#include "Application.h"

#include <cstdlib>
#include <gl/glut.h>

// No console window.
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  

void display() { Application::getSingleton().display(); }
void idle() { Application::getSingleton().idle(); }
void reshape(int w, int h) { Application::getSingleton().reshape(w, h); }

void processNormalKeys(unsigned char key, int x, int y) { Application::getSingleton().processNormalKeys(key, x, y); }
void processSpecialKeys(int key, int x, int y) { Application::getSingleton().processSpecialKeys(key, x, y); }

void processMouse(int button, int state, int x, int y) { Application::getSingleton().processMouse(button, state, x, y); }
void processMouseActiveMotion(int x, int y) { Application::getSingleton().processMouseActiveMotion(x, y); }

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("ai demo - shyam");

	new Application;

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);	

	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);

	glutMainLoop();

	delete Application::getSingletonPtr();

	return 0;
}