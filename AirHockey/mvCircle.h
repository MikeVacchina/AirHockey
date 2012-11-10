#ifndef MVCIRCLE
#define MVCIRCLE

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "mvObject.h"
#include "mvObjLoader.h"
#include "mvMesh.h"

//simple circle object with center and radius
//inherits object class
class mvCircle : public mvObject
{
public:
	mvCircle();
	~mvCircle();

	//reimplement loadMesh
	//TODO: this is an exact copy of the function in mvSphere
	//obviously that probably shouldn't happen
	void loadMesh(const char *filename);

	//public center and radius for now
	//TODO: also same as mvSphere
	glm::vec3 center;
	double radius;
};

#endif //MVCIRCLE