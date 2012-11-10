#ifndef MVPHYSICS
#define MVPHYSICS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <vector>

#include "mvMath.h"
#include "mvObject.h"

#include "mvSimpleStructs.h"

class mvPhysics
{
public:
	mvPhysics();

	~mvPhysics();

	//sets the gravity
	void setGravity(glm::vec3 g);

	//sets all objects
	void setObjs(std::vector<mvObject*> o);

	//clears all objects
	void clearObjs();

	//updates the physics and position of objects
	void update(double deltaTime);
	
private:
	glm::vec3 gravity;

	std::vector<mvObject*> objs;
};

#endif //MVPHYSICS