#include "mvTime.h"

mvTime::mvTime()
{
	started=false;
}

mvTime::~mvTime()
{
}

void mvTime::startTime()
{
	//set flag
	started = true;
	//get current time
	t1 = std::clock();//std::chrono::high_resolution_clock::now();
}

double mvTime::resetTime()
{
	if(started)
	{
		//get current time
		t2 = std::clock();//std::chrono::high_resolution_clock::now();
		//reset flag
		started = false;
		//get difference in times
		double rtn = double(t2-t1)/CLOCKS_PER_SEC;
		//start time again
		startTime();
		//return difference
		return rtn;
	}
	return -1.0;
}

double mvTime::lapTime()
{
	if(started)
	{
		//get current time
		t2 = std::clock();
		//reutnr difference in times
		return double(t2-t1)/CLOCKS_PER_SEC;
	}
	return -1.0;
}

double mvTime::stopTime()
{
	if(started)
	{
		//get current time
		t2 = std::clock();
		//reset flag
		started = false;
		//return difference in times
		return double(t2-t1)/CLOCKS_PER_SEC;
	}

	return -1.0;
}