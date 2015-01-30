#include "Application.h"
#include "Player.h"
#include "NPC.h"
#include "2D_Rendering.h"

#include <gl/glut.h>

#include <sstream>
#include <ctime>

Application::Application()
{
	glClearColor(0, 0, 0, 1);

	m_player = new Player(vec2(300, 300), 90);

	srand(time(NULL));

	for (int i = 0; i < 10; ++i)
	{
		float val0 = rand() / (float)(RAND_MAX - 1);
		float val1 = rand() / (float)(RAND_MAX - 1);
		float val2 = rand() / (float)(RAND_MAX - 1);

		m_npcs.push_back(new NPC(vec2(WINDOW_WIDTH * val0, WINDOW_HEIGHT * val1),
								 val2 * 360));
	}
}

Application::~Application()
{
	if (m_player)
		delete m_player;

	std::list< NPC* >::iterator iter;
	for (iter = m_npcs.begin(); iter != m_npcs.end(); ++iter)
	{
		delete (*iter);
	}
}

void Application::display()
{
	float now = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	static float prevTime = 0.0f;
	float deltaTime = now - prevTime;

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	m_player->update(deltaTime);
	m_player->display(deltaTime);

	std::list< NPC* >::iterator iter;
	for (iter = m_npcs.begin(); iter != m_npcs.end(); ++iter)
	{
		(*iter)->update(deltaTime);
		(*iter)->display(deltaTime);

		vec2 pos = (*iter)->getPosition();

		glPushMatrix();
		glColor3f(1, 1, 1);
		_2D::renderBitmapString(pos.x, pos.y, GLUT_BITMAP_HELVETICA_12, (*iter)->getCurrentStateName());
		glPopMatrix();
	}

	displayFPS();

	glFlush();
	glutSwapBuffers();

	prevTime = now;
}

void Application::idle()
{
	glutPostRedisplay();
}

void Application::displayFPS()
{
	static int frame = 0;
	static int timebase = 0;

	++frame;

	static float fps = 0;

	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000)
	{
		fps = frame * 1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
	}

	std::stringstream ss;
	ss << "FPS: " << fps;

	glColor3f(1, 1, 1);
	_2D::renderBitmapString(1, 11, GLUT_BITMAP_HELVETICA_12, ss.str().substr(0, 10));
}

void Application::reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void Application::processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		// ESC
		exit(-1);
		break;
	}
}

void Application::processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:

		break;
	}
}

void Application::processMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	//	m_player->setPosition(vec2(x, y));
		m_player->setTargetPos(vec2(x, y));
	}
}

void Application::processMouseActiveMotion(int x, int y)
{

}