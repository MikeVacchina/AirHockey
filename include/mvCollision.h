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
#include "mvTable.h"
#include "mvPuck.h"
#include "mvPaddle.h"

#include "mvMath.h"

class mvCollision
{
public:
	mvCollision();
	~mvCollision();
	
	//set a reference to objects in collision so they dont have to be passed in every call
	void setTable(mvTable *t);
	void setPuck(mvPuck *p);
	void setPaddle1(mvPaddle *p);
	void setPaddle2(mvPaddle *p);

	//clear references to objects
	void clearTable();
	void clearPuck();
	void clearPaddle1();
	void clearPaddle2();

	void clearObjs();

	//detect and resolve all collisions
	int resolveCollisions();

	//currently public bounciness value
	float bouncyness;

private:
	mvTable *table;
	mvPuck *puck;
	mvPaddle *paddle1;
	mvPaddle *paddle2;
	
	std::vector<mvWall> xWalls;
	std::vector<mvWall> zWalls;

	std::vector<mvWall> xGoals;
	std::vector<mvWall> zGoals;
};

#endif //MVCOLLISION