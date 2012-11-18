#ifndef MVPUCK
#define MVPUCK

#include "mvObject.h"

class mvPuck : public mvObject
{
public:
	mvPuck();
	~mvPuck();

	double radius;
};

#endif