#include "mvInput.h"


mvInput::mvInput()
{
	//set default values
	lMouse = MOUSE_INVALID;
	rMouse = MOUSE_INVALID;

	mouseSensitivity = 1.0;
}


mvInput::~mvInput()
{
}

void mvInput::handleMouseFunc(int button, int state, int x, int y, mvMouseData mouseInput)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			//set mouse state
			lMouse = MOUSE_DOWN;

			//initialize position and theda, phi
			init_x = x;
			init_y = y;
			init_pos = mouseInput.vel;
		}
		else if(state == GLUT_UP)
		{
			//set mouse state
			lMouse = MOUSE_UP;
		}
	}
}


bool mvInput::handleMouseMotionFunc(int x, int y, mvMouseData &mouseOutput)
{
	if(lMouse == MOUSE_DOWN)
	{
		//if left mouse is down set theda and phi base on motion of mouse from initial position
		glm::vec3 offset( (init_x-x)/mouseSensitivity , 0.0, (init_y-y)/mouseSensitivity );

		mouseOutput.vel = init_pos + offset;
		
		init_x = x;
		init_y = y;

		return true;
	}
	return false;
}

void mvInput::handleKeyboardFunc(unsigned char key, mvKeyboardData keyboardInput)
{
	if(keyboardInput.keyState == KEY_DOWN)
	{
		//exit on esc
		if(key == 27)
		{
			//clean up
			exit(0);
		}
		else
		{
			//set key to being pressed
			keyStates[key].setPressed(true);
		}
	}
	else if(keyboardInput.keyState == KEY_UP)
	{
		//set key to not being pressed
		keyStates[key].setPressed(false);
	}
}

void mvInput::handleSpecialFunc(int key, mvKeyboardData keyboardInput)
{
	if(keyboardInput.keyState == KEY_DOWN)
	{
		//set special key to being pressed
		specialStates[key].setPressed(true);
	}
	else if(keyboardInput.keyState == KEY_UP)
	{
		//set special key to not being pressed
		specialStates[key].setPressed(false);
	}
}
	
double mvInput::timeKeyDown(unsigned char key)
{
	//get length of time key was/is down
	return keyStates[key].timeDown();
}

double mvInput::timeSpecialDown(int key)
{
	//get length of time special key was/is down
	return specialStates[key].timeDown();
}
	
void mvInput::increaseMouseSensitivity()
{
	//increase mouse sensitivity
	mouseSensitivity *= 0.5;
}

void mvInput::decreaseMouseSensitivity()
{
	//decrease mouse sensitivity
	mouseSensitivity *= 2.0;
}

void mvInput::resetMouseSensitivity()
{
	//reset mouse sensitivity
	mouseSensitivity = 15.0;
}