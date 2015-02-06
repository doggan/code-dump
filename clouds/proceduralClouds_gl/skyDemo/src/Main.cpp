#include <cstdlib>
#include "gl/glew.h"
#include "gl/glut.h"

#include "Constants.h"
#include "Application.h"

// No console window.
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  

void display() { Application::getSingleton().display(); }
void idle() { Application::getSingleton().idle(); }
void reshape(int w, int h) { Application::getSingleton().reshape(w, h); }

void processNormalKeys(unsigned char key, int x, int y) { Application::getSingleton().processNormalKeys(key, x, y); }
void processSpecialKeys(int key, int x, int y) { Application::getSingleton().processSpecialKeys(key, x, y); }

int main(int argc, char **argv)
{
	new Application;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("skyDemo - 580");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	GLenum err = glewInit();
	if (GLEW_OK != err)
		return -1;

	if (!glewIsSupported("GL_VERSION_2_0"))
		return -1;

	if (!Application::getSingleton().init())
		return -1;

	glutMainLoop();

	delete Application::getSingletonPtr();

	return 0;
}

