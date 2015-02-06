#include "Application.h"

int main(int argc, char** args)
{
	new Application();

	if (Application::getSingleton().init() == false)
		return 1;

	Application::getSingleton().run();

	delete Application::getSingletonPtr();

	return 0;    
}