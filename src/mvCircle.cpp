#include "mvCircle.h"


mvCircle::mvCircle()
{
	//set default values
	center = glm::vec3(0.0);
	radius = 0.0;
}


mvCircle::~mvCircle()
{
}

//reimplement loadMesh function to also determine center and radius based on mesh
void mvCircle::loadMesh(const char *filename)
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
