#ifndef MVMAZE
#define MVMAZE

#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <fstream>
#include <iostream>

#include "mvObject.h"
#include "mvSimpleStructs.h"
#include "mvMesh.h"
#include "mvRect.h"
#include "mvCircle.h"

class mvMaze : public mvObject
{
public:
	mvMaze();
	~mvMaze();

	//loads special file that defines maze
	bool loadMaze(const char *filename);
	
	//sets basic rect as floor of desired size
	void setSize(double w, double d);

	//adds wall at position (x,z) scaled to have width w and depth d
	void setWall(double x, double z, double w, double d);

	//adds hole at position (x,z) with radius r
	void setHole(double x, double z, double r);

	//sets geometry and sorts walls
	void init();

	//returns x axis aligned walls
	std::vector<mvWall> getXWalls();
	//returns z axis aligned walls
	std::vector<mvWall> getZWalls();
	//returns holes
	std::vector<mvHole> getHoles();
	//returns goal as vec3 with vec3.x = x, vec3.y = z, vec3.z = r
	glm::vec3 getGoal();
	//returns start position
	glm::vec3 getBegin();

private:
	mvRect rect;
	
	std::vector<mvWall> xWalls;
	std::vector<mvWall> zWalls;

	std::vector<mvHole> holes;

	mvCircle begin;
	mvCircle goal;

	//helper functions to load special maze file
	bool skipCommentLine(std::istream& is);
	void skipLine(std::istream &is);
	bool processLine(std::istream &is);
};

#endif //MVMAZE