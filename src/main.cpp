#include <cstdlib>
#include <cstdio>

#include "framework_labyrinth.h"

int main(int argc, char **argv)
{
	//set start (default) window size
	int windowWidth = 640;
	int windowHeight = 480;

	//initialize glut
	initializeGlut(argc,argv);

	//initialize labyrinth game
	if(!framework_labyrinth::instance()->initialize("Labyrinth Game", windowWidth, windowHeight))
	{
		system("PAUSE");
		return -1;
	}

	//start glut main loop
	startGlut();

	return 1;
}