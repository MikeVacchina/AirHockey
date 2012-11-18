#include "mvPaddle.h"

mvPaddle::mvPaddle(void)
{
	radius = 0.91703;
}

mvPaddle::~mvPaddle(void)
{
}

void mvPaddle::fixMesh(int paddleID)
{
	if(paddleID==PADDLE1)
		mesh.translate(vec3(0.0,3.88387,0.0));
	else if(paddleID==PADDLE2)
		mesh.translate(vec3(0.0,-3.80908,0.0));

	geometry = mesh.getGeometry();
}