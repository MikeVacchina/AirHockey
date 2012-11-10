#include "mvCube.h"

mvCube::mvCube()
{
	std::string error;
	mvObjLoader("cube.obj", mesh, error);

	if(error != "")
	{
        std::cerr << "[ERROR] cube.obj FAILED TO LOAD: ";
		std::cerr << error;
		return;
	}

	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}


mvCube::~mvCube()
{
}

void mvCube::setColor(double r, double g, double b)
{
	mesh.setColor(r, g, b);
	geometry = mesh.getGeometry();
}