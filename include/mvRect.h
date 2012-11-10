#ifndef MVRECT
#define MVRECT

#include <iostream>

#include "mvObject.h"
#include "mvObjLoader.h"
#include "mvMesh.h"
#include "mvSimpleStructs.h"

//simple rect object that allows scaling and translation of mesh
//inherits object class
class mvRect : public mvObject
{
public:
	mvRect(const char *filename = NULL);
	~mvRect();
	
	//allow scaling and translation of mesh
	void scale(double x, double y, double z);
	void translate(double x, double y, double z);
};

#endif //MVRECT