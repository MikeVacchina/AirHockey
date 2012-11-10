#ifndef MVSPHERE
#define MVSPHERE

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "mvObject.h"
#include "mvObjLoader.h"
#include "mvMesh.h"
#include "mvSimpleStructs.h"

//simple sphere object with center and radius
//and allows scaling and translation of mesh
//inherits object class
class mvSphere : public mvObject
{
public:
	mvSphere(const char *filename = NULL);
	~mvSphere();
	
	//reimplement loadMesh
	void loadMesh(const char *filename);
	
	//allow scaling and translation of mesh
	void scale(double s);
	void translate(double x, double y, double z);

	//return mesh radius
	double getMeshRadius();

	//return mesh center
	void getMeshCenter(double &x, double &y, double &z);

	//indicator of the ball falling so collisions no longer happen
	//since collision are done in 2D
	bool falling;

private:
	double radius;
	glm::vec3 center;
};

#endif //MVSPHERE