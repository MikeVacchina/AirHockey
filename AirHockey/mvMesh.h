#ifndef MVMESH
#define MVMESH

#include <vector>
#include <cstdio>

#include "mvSimpleStructs.h"

//simple mesh class
class mvMesh
{
public:
	mvMesh();
	~mvMesh();
	
	void addVertex(vec3 v);
	void addFace(vec3 f);
	void addVertexNorm(vec3 vn);
	void addUV(vec2 vt);

	void addTexture(std::string file);

	bool hasTexture();

	GLuint getTextureID();
	
	void setColor(vec3 c);
	void setColor(double r, double g, double b);

	//create and return geometry
	mvVertex* getGeometry();
	int countVertices();

	//appends a mesh to this mesh
	void addMesh(mvMesh &mesh);
	
	//allow scaling and translation of mesh
	void scale(double s);
	void scale(vec3 s);
	void translate(vec3 v);

	//clears all current mesh data
	void clear();

private:
	std::vector<vec3> vertices;
	std::vector<vec3> vertexNorms;
	std::vector<vec3> faces;
	std::vector<vec3> colors;
	std::vector<vec2> UV;
	GLuint textureID;
};

#endif // MVMESH