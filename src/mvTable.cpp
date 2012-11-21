#include "mvTable.h"


mvTable::mvTable()
{
	//set default values to wall positions
	xWalls.push_back(mvWall(4.2659,6.0, 1));
	zWalls.push_back(mvWall(-6.74223,6.73572, 1));

	xWalls.push_back(mvWall(-6.0,-4.2575, 2));
	zWalls.push_back(mvWall(-6.74223,6.73572, 2));

	xWalls.push_back(mvWall(-4.2575,-1.3534, 3));
	zWalls.push_back(mvWall(6.73572,8.0, 3));

	xWalls.push_back(mvWall(1.36179,4.2659, 4));
	zWalls.push_back(mvWall(6.73572,8.0, 4));

	xWalls.push_back(mvWall(-4.2575,-1.3534, 5));
	zWalls.push_back(mvWall(-8.0,-6.74223, 5));

	xWalls.push_back(mvWall(1.36179,4.2659, 6));
	zWalls.push_back(mvWall(-8.0,-6.74223, 6));
	
	//set default values to goal positions
	xGoals.push_back(mvWall(-1.3534,1.36179, 1));
	zGoals.push_back(mvWall(6.73572,8.0, 1));
	
	xGoals.push_back(mvWall(-1.3534,1.36179, 2));
	zGoals.push_back(mvWall(-8.0,-6.74223, 2));
	
	std::sort(xWalls.begin(), xWalls.end());
	std::sort(zWalls.begin(), zWalls.end());

	std::sort(xGoals.begin(), xGoals.end());
	std::sort(zGoals.begin(), zGoals.end());
}

mvTable::~mvTable()
{
}

std::vector<mvWall> mvTable::getXWalls()
{
	return xWalls;
}

std::vector<mvWall> mvTable::getZWalls()
{
	return zWalls;
}

std::vector<mvWall> mvTable::getXGoals()
{
	return xGoals;
}

std::vector<mvWall> mvTable::getZGoals()
{
	return zGoals;
}