#include "mvRect.h"

mvRect::mvRect(const char *filename)
{
	//load file if there
	if(filename)
	{
		loadMesh(filename);
	}
}


mvRect::~mvRect()
{
}
	
void mvRect::scale(double x, double y, double z)
{
	//scale mesh
	mesh.scale(vec3(x,y,z));

	//reload geometry and vertex count
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}

void mvRect::translate(double x, double y, double z)
{
	//translate mesh
	mesh.translate(vec3(x,y,z));
	
	//reload geometry and vertex count
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}