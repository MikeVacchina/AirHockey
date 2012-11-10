#ifndef MVCUBE
#define MVCUBE

#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include <iostream>
#include <string>

#include "mvObject.h"
#include "mvSimpleStructs.h"
#include "mvObjLoader.h"
#include "mvMesh.h"

class mvCube : public mvObject
{
public:
	mvCube();
	~mvCube();

	void setColor(double r, double g, double b);

private:
	mvMesh mesh;
};

#endif //MVCUBE
