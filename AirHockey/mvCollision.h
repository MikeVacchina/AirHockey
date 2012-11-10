#ifndef MVCOLLISION
#define MVCOLLISION

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "defines.h"

#include "mvObject.h"
#include "mvMaze.h"
#include "mvSphere.h"

#include "mvMath.h"

class mvCollision
{
public:
	mvCollision();
	~mvCollision();
	
	//set a reference to objects in collision so they dont have to be passed in every call
	void setMaze(mvMaze *m);
	void setBall(mvSphere *b);

	//clear references to objects
	void clearMaze();
	void clearBall();

	//detect and resolve all collisions
	int resolveCollisions();

	//currently public bounciness value
	float bouncyness;

private:
	mvMaze *maze;
	mvSphere *ball;

	std::vector<mvWall> xWalls;
	std::vector<mvWall> zWalls;
	std::vector<mvHole> holes;
	glm::vec3 goal;
	double radius;
};

#endif //MVCOLLISION