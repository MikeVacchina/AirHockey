#include "mvPhysics.h"

mvPhysics::mvPhysics()
{
	gravity = glm::vec3(0.0);
}

mvPhysics::~mvPhysics()
{
}
	
void mvPhysics::setGravity(glm::vec3 g)
{
	//TODO: slightly hacky fix later
	gravity.x = g.x;
	gravity.z = g.z;
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
	glm::vec3 A;

	for(int i=0,sizei=objs.size();i<sizei;++i)
	{
		A = objs[i]->acc + gravity;

		//check for normal forces and if there is some do not allow acceleration along that force
		for(int j=0,sizej=objs[i]->normalForces.size();j<sizej;++j)
		{
			//check if acceleration goes against normal force
			if(glm::dot(A, objs[i]->normalForces[j]) < 0)
			{
				//remove normal force direction from acceleration
				glm::vec3 c = glm::cross(A, objs[i]->normalForces[j]);
				glm::vec3 nA = glm::cross(objs[i]->normalForces[j], c);

				//make sure new accerleration isn't 0 before rescaling
				if(nA != glm::vec3(0.0))
				{
					//rescale new acceleration to proper length
					double s = glm::dot(A, nA) / nA.length();
					nA = glm::normalize(nA);
					nA *= s;
				}
				A = nA;
			}
		}

		//update velocity and position
		objs[i]->vel += T * A;
		objs[i]->pos += T * objs[i]->vel;

		//reset normal forces
		objs[i]->normalForces.clear();
	}
}