#ifndef MVPADDLE
#define MVPADDLE

#include "mvObject.h"
#include "defines.h"

class mvPaddle : public mvObject
{
public:
	mvPaddle();
	~mvPaddle();

	void fixMesh(int paddleID);
	
	double radius;
};

#endif