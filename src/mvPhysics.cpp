#include "mvPhysics.h"

mvPhysics::mvPhysics()
{
}

mvPhysics::~mvPhysics()
{
}

void mvPhysics::setObjs(std::vector<mvObject*> o)
{
	objs = o;
}

void mvPhysics::clearObjs()
{
	objs.clear();
}

void mvPhysics::update(double deltaTime)
{
	glm::vec3 T(deltaTime);

	for(int i=0,sizei=objs.size();i<sizei;++i)
	{
		//update velocity and position
		objs[i]->pos += T * objs[i]->vel;
	}
}