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

//TODO: fix collisions for new objects
int mvCollision::resolveCollisions()
{
	//if ball or maze refernces not set return invalid
	if(table == NULL || puck == NULL || paddle1 == NULL || paddle2 == NULL)
		return INVALID;

	//flag for if the ball is in goal
	bool reachedGoal=false;

	std::set<int> xWallsSet;
	std::set<int> zWallsSet;
	std::map<int,std::pair<int,int> > walls;
	std::set<int> intersections;

	//check for a collision

	//TODO
	//check if in goal

	//check collision with paddles
	double paddlePuckDist = sqrt((puck->pos.x-paddle1->pos.x)*(puck->pos.x-paddle1->pos.x) + (puck->pos.z-paddle1->pos.z)*(puck->pos.z-paddle1->pos.z));

	if(paddlePuckDist <= puck->radius + paddle1->radius)
	{
		//puck collided with paddle

		//bounce the puck off the wall using the norm to find correct direction
		
		glm::vec3 combinedVel = puck->vel - paddle1->vel;
		glm::vec3 norm = puck->pos - paddle1->pos;
		glm::vec3 orth = glm::vec3(-norm.z,0.0,-norm.x);

		double d = paddlePuckDist - puck->radius - paddle1->radius;

		d /= -2.0;
		
		norm = glm::normalize(norm);
		orth = glm::normalize(orth);

		puck->pos = puck->pos + norm*glm::vec3(d);
		
		double nV_Paddle;
		double nV_Puck, oV_Puck;
		
		nV_Paddle = glm::dot(paddle1->vel,norm);
		nV_Puck = glm::dot(puck->vel,norm);

		oV_Puck = glm::dot(puck->vel,orth);

		if(nV_Paddle < 0)
			nV_Paddle = 0;
		
		nV_Puck = abs(nV_Puck) + nV_Paddle;

		puck->vel = orth*glm::vec3(oV_Puck) + norm*glm::vec3(nV_Puck);
	}

	//check collision with walls

	//search through x axis aligned walls
	for(int i=0,sizei=xWalls.size();i<sizei;++i)
	{
		if(puck->pos.x + puck->radius > xWalls[i].start && puck->pos.x - puck->radius < xWalls[i].end)
		{
			xWallsSet.insert(xWallsSet.end(),xWalls[i].id);
			walls[xWalls[i].id].first = i;
		}
		else if(puck->pos.x + puck->radius < xWalls[i].start)
			break;
	}

	//search through z axis aligned walls
	for(int i=0,sizei=zWalls.size();i<sizei;++i)
	{
		if(puck->pos.z + puck->radius > zWalls[i].start && puck->pos.z - puck->radius < zWalls[i].end)
		{
			zWallsSet.insert(zWallsSet.end(),zWalls[i].id);
			walls[zWalls[i].id].second = i;
		}
		else if(puck->pos.z + puck->radius < zWalls[i].start)
			break;
	}

	//get walls were there was an intersection in both x and z directions
	std::set_intersection(xWallsSet.begin(), xWallsSet.end(), zWallsSet.begin(), zWallsSet.end(), std::inserter(intersections, intersections.end()));
	
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
			//center is in wall

			//currently do not do anything
			//unless the computer sucks the puck should never move fast enough that in a time step it is suddenly in a wall
				
			//ignore this following commented out code as it is incomplete

			//get line segment(s) it intersects with
				//using commented out section below
			//int segID = getIntersectingSegment(wallPts, glm::vec2(lP.x, lP.y),glm::vec2(P.x, P.y));

			//get normal to line segment(s)
			//bounce using normal

			//glm::vec2 motionFirst(lP.x, lP.z);
			//glm::vec2 motionSecond(P.x-lP.x, P.z-lP.z);

			//std::vector<glm::vec2> wallPts;

			//std::set<int>::iterator it = intersections.begin();
			//
			//wallPts.push_back(glm::vec2( xWalls[walls[*it].first].start, zWalls[walls[*it].second].start));
			//wallPts.push_back(glm::vec2( xWalls[walls[*it].first].start, zWalls[walls[*it].second].end));
			//wallPts.push_back(glm::vec2( xWalls[walls[*it].first].end, zWalls[walls[*it].second].end));
			//wallPts.push_back(glm::vec2( xWalls[walls[*it].first].end, zWalls[walls[*it].second].start));

			//for(int i=0,size=wallPts.size();i<size;++i)
			//{
			//	int next = i+1==size?0:i+i;
			//	double top = glm::dot((motionFirst - wallPts[i]),wallPts[next]);
			//	double btm = glm::dot(motionSecond, wallPts[next]);
			//	double val;

			//	if(btm == 0)
			//	{
			//		//parallel :(
			//		if(top==0)
			//		{
			//			//colinear :|
			//		}
			//	}
			//	else
			//	{
			//		val = top/btm;
			//		if(val <= 1 && val >= 0)
			//		{
			//			//found segment of collision
			//			glm::vec2 ptCollision = motionFirst + motionSecond*glm::vec2(val);
			//		}
			//	}
			//}
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