#ifndef MVTIME
#define MVTIME

//#include <chrono>
#include <ctime>

class mvTime
{
public:
	mvTime();
	~mvTime();

	//start timer
	void startTime();

	//reset time and return elapse time
	double resetTime();

	//return elapse time
	double lapTime();

	//stop timer and return elapse time
	double stopTime();

private:
	std::clock_t t1, t2;
	bool started;
};

#endif //MVTIME