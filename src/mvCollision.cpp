#include "mvCollision.h"


mvCollision::mvCollision()
{
	//set default values
	table = NULL;
	puck = NULL;
	paddle1 = NULL;
	paddle2 = NULL;

	bouncyness = 0.5;
}


mvCollision::~mvCollision()
{
}

void mvCollision::setTable(mvTable *t)
{
	//set maze reference
	table = t;
	//get walls on table
	xWalls = table->getXWalls();
	zWalls = table->getZWalls();
	//get goals on table
	xGoals = table->getXGoals();
	zGoals = table->getZGoals();
}
void mvCollision::setPuck(mvPuck *p)
{
	puck = p;
}

void mvCollision::setPaddle1(mvPaddle *p)
{
	paddle1 = p;
}

void mvCollision::setPaddle2(mvPaddle *p)
{
	paddle2 = p;
}

void mvCollision::clearTable()
{
	//clear table reference
	table = NULL;
	//clear walls on table
	xWalls.clear();
	zWalls.clear();
	//clear goals on table
	xGoals.clear();
	zGoals.clear();
}

void mvCollision::clearPuck()
{
	puck = NULL;
}

void mvCollision::clearPaddle1()
{
	paddle1 = NULL;
}

void mvCollision::clearPaddle2()
{
	paddle2 = NULL;
}

void mvCollision::clearObjs()
{
	clearTable();
	clearPuck();
	clearPaddle1();
	clearPaddle2();
}

//TODO: currently function is ugly 
//needs to be broken down into parts
//collision detection with the multiple objects
//followed by collision response
int mvCollision::resolveCollisions()
{
	//if ball or maze refernces not set return invalid
	if(table == NULL || puck == NULL || paddle1 == NULL || paddle2 == NULL)
		return INVALID;

	//flag for if the ball is in goal
	bool reachedGoal=false;
	
	std::set<int> xWallsSet;
	std::set<int> zWallsSet;
	std::set<int> xGoalsSet;
	std::set<int> zGoalsSet;
	std::map<int,std::pair<int,int> > walls;
	std::map<int,std::pair<int,int> > goals;
	std::set<int> intersections;
	std::set<int> intersectionsGoal;

	//Ensure Paddles are confined to their spaces
	if(paddle1->pos.x + paddle1->radius > xWalls[xWalls.size()-2].end)
		paddle1->pos.x = xWalls[xWalls.size()-2].end - paddle1->radius;
	if(paddle1->pos.x - paddle1->radius < xWalls[1].start)
		paddle1->pos.x = xWalls[1].start + paddle1->radius;
	if(paddle1->pos.z - paddle1->radius < zWalls[1].end)
		paddle1->pos.z = zWalls[1].end + paddle1->radius;
	if(paddle1->pos.z + paddle1->radius > 0)
		paddle1->pos.z = -paddle1->radius;
	
	if(paddle2->pos.x + paddle2->radius > xWalls[xWalls.size()-2].end)
		paddle2->pos.x = xWalls[xWalls.size()-2].end - paddle2->radius;
	if(paddle2->pos.x - paddle2->radius < xWalls[1].start)
		paddle2->pos.x = xWalls[1].start + paddle2->radius;
	if(paddle2->pos.z + paddle2->radius > zWalls[zWalls.size()-2].start)
		paddle2->pos.z = zWalls[zWalls.size()-2].start - paddle2->radius;
	if(paddle2->pos.z - paddle2->radius < 0)
		paddle2->pos.z = paddle2->radius;

	//check for a collision

	getWallIntersection(puck->pos, puck->radius, xGoals, zGoals, goals, intersectionsGoal);
	
	//each intersection is a collision with a wall
	for(std::set<int>::iterator it=intersectionsGoal.begin();it!=intersectionsGoal.end();++it)//there has been a collision!
	{
		int index = (*it);

		//check if puck is in goal rather than on edge of it
		if(puck->pos.x > xGoals[goals[index].first].start && puck->pos.x < xGoals[goals[index].first].end && puck->pos.z > zGoals[goals[index].second].start && puck->pos.z < zGoals[goals[index].second].end)
		{
			//TODO: make it dissappear
			if(xGoals[goals[index].first].id == 1)
				return GOAL1;
			if(xGoals[goals[index].first].id == 2)
				return GOAL2;
		}
	}
	
	resolvePaddleCollision(puck->pos,puck->radius,puck->vel,paddle1->pos,paddle1->radius, paddle1->vel);

	resolvePaddleCollision(puck->pos,puck->radius,puck->vel,paddle2->pos,paddle2->radius, paddle2->vel);

	//check collision with walls
	getWallIntersection(puck->pos, puck->radius, xWalls, zWalls, walls, intersections);
	
	//each intersection is a collision with a wall
	for(std::set<int>::iterator it=intersections.begin();it!=intersections.end();++it)//there has been a collision!
	{
		//resolve collision

		int index = (*it);

		//get segments of wall
		std::vector<std::pair<glm::vec2,glm::vec2> > wallPts;

		wallPts.push_back(std::pair<glm::vec2,glm::vec2>(glm::vec2( xWalls[walls[index].first].start, zWalls[walls[index].second].start),
														glm::vec2( xWalls[walls[index].first].start, zWalls[walls[index].second].end)));

		wallPts.push_back(std::pair<glm::vec2,glm::vec2>(glm::vec2( xWalls[walls[index].first].start, zWalls[walls[index].second].end),
														glm::vec2( xWalls[walls[index].first].end, zWalls[walls[index].second].end)));
			
		wallPts.push_back(std::pair<glm::vec2,glm::vec2>(glm::vec2( xWalls[walls[index].first].end, zWalls[walls[index].second].end),
														glm::vec2( xWalls[walls[index].first].end, zWalls[walls[index].second].start)));

		wallPts.push_back(std::pair<glm::vec2,glm::vec2>(glm::vec2( xWalls[walls[index].first].end, zWalls[walls[index].second].start),
														glm::vec2( xWalls[walls[index].first].start, zWalls[walls[index].second].start)));

		//set default norm
		glm::vec2 norm(0.0);
			
		//check if puck is in wall
		if(puck->pos.x > xWalls[walls[index].first].start && puck->pos.x < xWalls[walls[index].first].end && puck->pos.z > zWalls[walls[index].second].start && puck->pos.z < zWalls[walls[index].second].end)
		{
		}
		else
		{
			//radius of puck intersected a wall

			//get the closest wall segment with the puck

			//first segment is default closest
			double dist =  puck->radius - distanceLineSegPt(wallPts[0].first,wallPts[0].second,glm::vec2(puck->pos.x, puck->pos.z));

			//keep track of the segment(s) that are closest
			std::vector<int> intersectedSegments;
			intersectedSegments.push_back(0);

			for(int i=1;i<4;++i)
			{
				//get distance from center of sphere to line segment 
				double d = puck->radius - distanceLineSegPt(wallPts[i].first,wallPts[i].second,glm::vec2(puck->pos.x, puck->pos.z));
				
				//biggest d will be closest since we are doing radius - distance
				if(d > dist)
				{
					intersectedSegments.clear();
					intersectedSegments.push_back(i);
					dist = d;
				}
				else if(d == dist)
				{
					//puck can be hitting two segments at same time...
					//so add to tracked segments
					intersectedSegments.push_back(i);
				}
			}

			//check for valid distance
			if(dist < 0)
			{
				//didnt actually collide

				//this happens because the puck is a sphere and we are doing axis aligned search to find collisions
				//happens when puck is close to hitting a corner of a wall

				return NONE;
			}

			//get normal to line segment(s)
			for(int i=0, size=intersectedSegments.size();i<size;++i)
			{
				glm::vec2 tmpNorm;
				//get orthogonal vector to segment
				tmpNorm.x = -1*(wallPts[intersectedSegments[i]].first.y - wallPts[intersectedSegments[i]].second.y);
				tmpNorm.y = -1*(wallPts[intersectedSegments[i]].first.x - wallPts[intersectedSegments[i]].second.x);

				//normalize vector
				tmpNorm = glm::normalize(tmpNorm);

				//determine the direction of the normal
				if(glm::dot(tmpNorm,wallPts[intersectedSegments[i]].first-glm::vec2(puck->pos.x,puck->pos.z)) > 0)
					tmpNorm *= -1;

				//add the normal as a normal force acting on the puck for the next physics update
				puck->normalForces.push_back(glm::vec3(tmpNorm.x, 0.0, tmpNorm.y));

				//sum up normals
				norm += tmpNorm;
			}

			//average normals
			norm /= (double)intersectedSegments.size();

			//ensure it is normalized
			norm = glm::normalize(norm);

			//set offset to keep puck out of the walls
			glm::vec2 offset(norm.x*dist, norm.y*dist);

			//move puck out of wall
			puck->pos = glm::vec3(offset.x + puck->pos.x, 0.0, offset.y + puck->pos.z);
		}

		//bounce the puck off the wall using the norm to find correct direction

		double u = -2*glm::dot(puck->vel,glm::vec3(norm.x, 0.0, norm.y));

		glm::vec3 nV = glm::normalize( puck->vel + glm::vec3(norm.x*u, 0.0, norm.y*u) );
		
		nV = glm::normalize(nV);

		//scale back the velocity as momentom and energy is lost
		float scale = glm::length(puck->vel)*bouncyness;
			
		nV.x = nV.x * scale;
		nV.y = nV.y * scale;
		nV.z = nV.z * scale;

		//set new velocity
		puck->vel = nV;
	}

	//return proper result
	if(reachedGoal)
		return GOAL1;
	return NONE;
}

void mvCollision::getWallIntersection(glm::vec3 pos, double radius, std::vector<mvWall> &xWalls, std::vector<mvWall> &zWalls, std::map<int,std::pair<int,int> > &walls, std::set<int> &intersections)
{
	std::set<int> xWallsSet;
	std::set<int> zWallsSet;

	//search through x axis aligned walls
	for(int i=0,sizei=xWalls.size();i<sizei;++i)
	{
		if(pos.x + radius > xWalls[i].start && pos.x - radius < xWalls[i].end)
		{
			xWallsSet.insert(xWallsSet.end(),xWalls[i].id);
			walls[xWalls[i].id].first = i;
		}
		else if(pos.x + radius < xWalls[i].start)
			break;
	}

	//search through z axis aligned walls
	for(int i=0,sizei=zWalls.size();i<sizei;++i)
	{
		if(pos.z + radius > zWalls[i].start && pos.z - radius < zWalls[i].end)
		{
			zWallsSet.insert(zWallsSet.end(),zWalls[i].id);
			walls[zWalls[i].id].second = i;
		}
		else if(pos.z + radius < zWalls[i].start)
			break;
	}

	//get walls were there was an intersection in both x and z directions
	std::set_intersection(xWallsSet.begin(), xWallsSet.end(), zWallsSet.begin(), zWallsSet.end(), std::inserter(intersections, intersections.end()));
}

void mvCollision::resolvePaddleCollision(glm::vec3 &pos, double radius, glm::vec3 &vel, glm::vec3 p, double r, glm::vec3 v)
{
	//check collision with paddles
	double paddlePuckDist = sqrt((pos.x-p.x)*(pos.x-p.x) + (pos.z-p.z)*(pos.z-p.z));

	if(paddlePuckDist <= radius + r)
	{
		//puck collided with paddle
		
		glm::vec3 combinedVel = vel - v;
		glm::vec3 norm = pos - p;
		glm::vec3 orth = glm::vec3(-norm.z,0.0,-norm.x);

		double d = paddlePuckDist - radius - r;

		d /= -2.0;
		
		norm = glm::normalize(norm);
		orth = glm::normalize(orth);

		pos = pos + norm*glm::vec3(d);
		
		double nV_Paddle;
		double nV_Puck, oV_Puck;
		
		nV_Paddle = glm::dot(v,norm);
		nV_Puck = glm::dot(vel,norm);

		oV_Puck = glm::dot(vel,orth);

		if(nV_Paddle < 0)
			nV_Paddle = 0;
		
		nV_Puck = abs(nV_Puck) + nV_Paddle;

		vel = orth*glm::vec3(oV_Puck) + norm*glm::vec3(nV_Puck);
	}
}