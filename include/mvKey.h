#ifndef MVKEY
#define MVKEY

#include "mvTime.h"

//an instance of this class represents a key
class mvKey
{
public:
	mvKey();

	~mvKey();

	//checks if the key is down
	bool isDown();

	//returns how long is was/is down so far
	double timeDown();

	//sets whether the key is pressed or not
	void setPressed(bool down);

private:
	mvTime stopwatch;
	bool pressed;
	double downTime;
};

#endif //MVKEY