#include "mvSphere.h"

mvSphere::mvSphere(const char *filename)
{
	//set default values
	radius = 0.0;
	center = glm::vec3(0.0);
	falling = false;

	//load file if there
	if(filename)
	{
		loadMesh(filename);
	}
}


mvSphere::~mvSphere()
{
}

//reimplement loadMesh function to also determine center and radius based on mesh
void mvSphere::loadMesh(const char *filename)
{
	std::string error;
	
	//load mesh
	mvObjLoader(filename, mesh, error);
	
	//check for error
	if(error != "")
	{
        std::cerr << "[ERROR] " << filename << " FAILED TO LOAD";
		std::cerr << error;
		return;
	}
	
	//set geometry and vertex count based on mesh
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
	
	//reset radius and center
	radius = 0.0;
	center = glm::vec3(0.0);

	for(int i=0;i<vertexCount;++i)
	{
		//add up vertices for center
		center.x += geometry[i].position[0];
		center.y += geometry[i].position[1];
		center.z += geometry[i].position[2];
	}

	//average
	center.x /= vertexCount;
	center.y /= vertexCount;
	center.z /= vertexCount;

	for(int i=0;i<vertexCount;++i)
	{
		//add up distances for radius
		double tmp = (geometry[i].position[0]-center.x)*(geometry[i].position[0]-center.x) +
					 (geometry[i].position[1]-center.y)*(geometry[i].position[1]-center.y) +
					 (geometry[i].position[2]-center.z)*(geometry[i].position[2]-center.z);
		tmp = sqrt(tmp);
		radius += tmp;
	}

	//average
	radius /= vertexCount;
}
	
void mvSphere::scale(double s)
{
	//scale mesh
	mesh.scale(s);

	//scale radius
	radius *= s;

	//scale center
	center.x *= float(s);
	center.y *= float(s);
	center.z *= float(s);

	//reload geometry and vertex count
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}

void mvSphere::translate(double x, double y, double z)
{
	//translate mesh
	mesh.translate(vec3(x,y,z));

	//translate center
	center.x += float(x);
	center.y += float(y);
	center.z += float(z);

	//reload geometry and vertex count
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}

double mvSphere::getMeshRadius()
{
	//return mesh radius
	return radius;
}

void mvSphere::getMeshCenter(double &x, double &y, double &z)
{
	//return mesh center
	x = center.x;
	y = center.y;
	z = center.z;
}