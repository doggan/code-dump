#include "Application.h"

#ifndef EDITOR

int main(int argc, char** args)
{
	try
	{
		new Application();
		Application::getSingleton().init();
		Application::getSingleton().run();
		delete Application::getSingletonPtr();
	}
	catch (Exception &e)
	{
		e.messageBox();
		return 1;
	}
	
	return 0;    
}

#endif