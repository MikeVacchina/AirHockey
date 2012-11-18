#include "mvTable.h"


mvTable::mvTable()
{
	xWalls.push_back(mvWall(4.2659,4.5540, 1));
	zWalls.push_back(mvWall(-6.74223,6.73572, 1));

	xWalls.push_back(mvWall(-4.5456,-4.2575, 2));
	zWalls.push_back(mvWall(-6.74223,6.73572, 2));

	xWalls.push_back(mvWall(-4.2575,-1.3534, 3));
	zWalls.push_back(mvWall(6.73572,6.9681, 3));

	xWalls.push_back(mvWall(1.36179,4.2659, 4));
	zWalls.push_back(mvWall(6.73572,6.9681, 4));

	xWalls.push_back(mvWall(-4.2575,-1.3534, 5));
	zWalls.push_back(mvWall(-6.9746,-6.74223, 5));

	xWalls.push_back(mvWall(1.36179,4.2659, 6));
	zWalls.push_back(mvWall(-6.9746,-6.74223, 6));
	
	
	xGoals.push_back(mvWall(-1.3534,1.36179, 1));
	zGoals.push_back(mvWall(6.73572,6.9681, 1));
	
	xGoals.push_back(mvWall(-1.3534,1.36179, 2));
	zGoals.push_back(mvWall(-6.9746,-6.74223, 2));
	
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