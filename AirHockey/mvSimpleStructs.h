#ifndef MVSIMPLESTRUCTS
#define MVSIMPLESTRUCTS

#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include "defines.h"

//basic vertex structure for glut
struct mvVertex
{
    GLfloat position[3];
    GLfloat color[3];
    GLfloat normal[3];
	GLfloat uv[2];
};

//mouse data to pass things from framework to input and vice versa
class mvMouseData
{
public:
	mvMouseData(double _theda=0.0, double _phi=0.0)
	{
		theda = _theda;
		phi   = _phi;
	}

	double theda;
	double phi;
};

//keyboard data to pass things from framework to input and vice versa
class mvKeyboardData
{
public:
	mvKeyboardData(enum _keyState ks = KEY_INVALID)
	{
		keyState = ks;
	}

	int keyState;
};

//simple vec3 used in mesh for vertices
class vec3
{
public:
	vec3(double _x=0, double _y=0, double _z=0)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	vec3 plus(int val)
	{
		return vec3(x+val, y+val, z+val);
	}

	vec3 plus(vec3 val)
	{
		return vec3(x+val.x, y+val.y, z+val.z);
	}

	vec3 mulScalar(double val)
	{
		return vec3(x*val, y*val, z*val);
	}

	vec3 mul(vec3 val)
	{
		return vec3(x*val.x, y*val.y, z*val.z);
	}

	double x, y, z;
};

class vec2
{
public:
	vec2(double _x=0, double _y=0)
	{
		x = _x;
		y = _y;
	}

	vec2 plus(int val)
	{
		return vec2(x+val, y+val);
	}

	vec2 plus(vec2 val)
	{
		return vec2(x+val.x, y+val.y);
	}

	vec2 mulScalar(double val)
	{
		return vec2(x*val, y*val);
	}

	vec2 mul(vec2 val)
	{
		return vec2(x*val.x, y*val.y);
	}

	double x, y;
};

//single axis wall that would be paired with 
//another single axis wall of the other axis
class mvWall
{
public:
	mvWall(double s=0, double e=0, int i=-1)
	{
		start = s;
		end = e;
		id = i;
	}

	bool operator<(mvWall &other)
	{
		return start < other.start;
	}

	double start, end;
	int id;
};

//simple hole structure with position and radius
class mvHole
{
public:
	mvHole(double _x=0.0, double _z=0.0, double _r=0.0)
	{
		x = _x;
		z = _z;
		r = _r;
	}

	double x, z, r;
};

#endif //MVSIMPLESTRUCTS

