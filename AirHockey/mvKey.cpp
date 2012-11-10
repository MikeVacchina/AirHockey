#include "mvKey.h"

mvKey::mvKey()
{
	//set default values
	pressed = false;
	downTime = 0.0;
}

mvKey::~mvKey()
{
}

bool mvKey::isDown()
{
	return pressed;
}

double mvKey::timeDown()
{
	//if down
	if(pressed)
		//return elapse time and reset timer
		return stopwatch.resetTime();
	else
	{
		//return last measure of downTime and reset downTime
		double rtn = downTime;
		downTime = 0.0;
		return rtn;
	}
}

void mvKey::setPressed(bool down)
{
	//if already correctly set do nothing
	if(pressed != down)
	{
		pressed = down;

		if(pressed)
			//start time if pressed
			stopwatch.startTime();
		else
			//set down time if unpressed
			downTime = stopwatch.stopTime();
	}
}