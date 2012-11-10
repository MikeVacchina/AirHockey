#ifndef MVOBJECT
#define MVOBJECT

#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <iostream>
#include <vector>

#include "mvSimpleStructs.h"
#include "mvObjLoader.h"
#include "mvMesh.h"

#include "assimpObjLoader.h"

//simple object class that all other objects should inherit from
//NOTE: probably will experience problems if mesh is changed after loading it into buffer for glut
class mvObject
{
public:
	mvObject();
	~mvObject();
	
	GLuint vbo_geometry;// VBO handle for our geometry
	glm::mat4 model;//obj->world each object should have its own model matrix

	//geometry and vectex count are based on mesh
	//and need to be reset each time mesh changes
	mvVertex *geometry;
	int vertexCount;
	
	//basic loadMesh function
	virtual void loadMesh(const char *filename);

	//assimp loadMesh function
	virtual void assimpLoadMesh(const char *filename);

	//basic getMesh function (returns a copy)
	virtual mvMesh getMesh();

	//sets the color of the entire mesh and reloads geometry
	virtual void setColor(double r, double g, double b);

	virtual bool hasTexture();

	//gets size of geometry for glut / opengl
	virtual int sizeofgeometry();

	virtual void bind();
	
	//physics variables
	glm::vec3 acc;
	glm::vec3 vel;
	glm::vec3 pos;

	//normal forces acting on the object
	std::vector<glm::vec3> normalForces;

protected:
	mvMesh mesh;

	bool textured;
	GLuint textureID;
};

#endif //MVOBJECT

