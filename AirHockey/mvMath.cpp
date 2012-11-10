#include "mvMath.h"

double distanceLineSegPt(glm::vec2 segA, glm::vec2 segB, glm::vec2 pt)
{
	//fine closest point on line to pt
	double top = (pt.x - segA.x)*( segB.x - segA.x ) + ( pt.y - segA.y )*( segB.y - segA.y );
	double btm = (segB.x-segA.x)*(segB.x-segA.x) + (segB.y - segA.y)*(segB.y - segA.y);
	double u = top/btm;

	//check if that point is on the line segment
	if(u>1 || u<0)
	{
		//get distance to each end of line segment
		double distA = (pt.x - segA.x)*(pt.x - segA.x) + (pt.y - segA.y)*(pt.y - segA.y);
		double distB = (pt.x - segB.x)*(pt.x - segB.x) + (pt.y - segB.y)*(pt.y - segB.y);

		//return closer distance
		if(distA > distB)
			return std::sqrt(distB);
		else
			return std::sqrt(distA);
	}
	else
	{
		//get closest point on line segment to pt
		glm::vec2 closestPt = segA + glm::vec2(u)*(segB - segA);

		//get distance between the point and pt
		return std::sqrt( (pt.x - closestPt.x)*(pt.x - closestPt.x) + (pt.y - closestPt.y)*(pt.y - closestPt.y) );
	}
}