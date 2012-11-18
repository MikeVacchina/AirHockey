#ifndef MVTABLE
#define MVTABLE

#include <vector>
#include <algorithm>

#include "mvObject.h"
#include "mvSimpleStructs.h"

class mvTable : public mvObject
{
public:
	mvTable();
	~mvTable();
	
	//returns x axis aligned walls
	std::vector<mvWall> getXWalls();
	//returns z axis aligned walls
	std::vector<mvWall> getZWalls();
	
	//returns x axis aligned goals
	std::vector<mvWall> getXGoals();
	//returns z axis aligned goals
	std::vector<mvWall> getZGoals();

private:
	std::vector<mvWall> xWalls;
	std::vector<mvWall> zWalls;

	std::vector<mvWall> xGoals;
	std::vector<mvWall> zGoals;
};

#endif