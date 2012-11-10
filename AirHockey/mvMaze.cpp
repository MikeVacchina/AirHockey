#include "mvMaze.h"

mvMaze::mvMaze()
{
	//load basic default cube object
	rect.loadMesh("cube.obj");
}

mvMaze::~mvMaze()
{
}

bool mvMaze::loadMaze(const char *filename)
{
	//open file
	std::ifstream ifs(filename, std::ios::in);
	
	//check for error
	if(!ifs)
	{
        std::cerr << "[ERROR] " << filename << " FAILED TO LOAD";
		return false;
	}

	
	xWalls.clear();
	zWalls.clear();
	holes.clear();
	mesh.clear();

	//load obj
	while(skipCommentLine(ifs))
	{
		if(!processLine(ifs))
			break;
	}
	
	//set geometry and sorts walls
	init();

	return true;
}

void mvMaze::setSize(double w, double d)
{
	//copy rect (basic cube)
	mvRect tmp;
	tmp = rect;
	
	//translate so top is at 0
	tmp.translate(0.0,-1.0,0.0);

	//scale to appropriate size
	tmp.scale(w/2.0,0.25,d/2.0);

	//set to green
	tmp.setColor(0.0,1.0,0.0);

	//set mesh (will lose any walls)
	mesh = tmp.getMesh();
	xWalls.clear();
	zWalls.clear();
}

void mvMaze::setWall(double x, double z, double w, double d)
{
	mvRect tmp;

	//get current wall count;
	int wallCount = xWalls.size();

	//copy rect (basic cube)
	tmp = rect;
	
	//scale to appropriate size
	tmp.scale(w/2.0,0.5,d/2.0);

	//translate to appropriate spot
	tmp.translate(x,0.5,z);

	//set walls to blue
	tmp.setColor(0.0,0.0,1.0);

	//append mesh
	mesh.addMesh(tmp.getMesh());
	
	//add to axis aligned wall vectos
	xWalls.push_back(mvWall(x-(w/2), x+(w/2.0), wallCount+1));
	zWalls.push_back(mvWall(z-(d/2), z+(d/2.0), wallCount+1));
}

void mvMaze::setHole(double x, double z, double r)
{
	//add hole
	holes.push_back(mvHole(x, z, r));
}

void mvMaze::init()
{
	//sort walls
	std::sort(xWalls.begin(), xWalls.end());
	std::sort(zWalls.begin(), zWalls.end());

	//load geometry and vertex count
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}

std::vector<mvWall> mvMaze::getXWalls()
{
	return xWalls;
}

std::vector<mvWall> mvMaze::getZWalls()
{
	return zWalls;
}

std::vector<mvHole> mvMaze::getHoles()
{
	return holes;
}

glm::vec3 mvMaze::getGoal()
{
	return glm::vec3(goal.center.x, goal.center.z, goal.radius);
}

glm::vec3 mvMaze::getBegin()
{
	return begin.center;
}

bool mvMaze::skipCommentLine(std::istream& is)
{
	char next;

	//set skip white space
	while(is >> std::skipws >> next)
	{
		is.putback(next);
		//check for comment
		if(next == '#')
			skipLine(is);
		else
			return true;
	}
	return false;
}

void mvMaze::skipLine(std::istream &is)
{
	char next;

	//do not skip white space
	is >> std::noskipws;

	//load line and ignore all of it
	while( (is>>next) && (next != '\n') );
}
	
bool mvMaze::processLine(std::istream &is)
{
	std::string ele;
	
	//check for eof
	if( !(is >> ele) )
		return false;

	if(ele == "f")//floor obj
	{
		//should be first thing in maze file
		std::string tmp;

		//get filename
		is >> tmp;
		
		//load obj
		mvObject floor;
		floor.loadMesh(tmp.c_str());

		//set color to teal
		floor.setColor(0.0,1.0,1.0);

		//add mesh
		mesh.addMesh(floor.getMesh());
	}
	else if(ele == "c")//basic cube obj
	{
		std::string tmp;

		// get filename
		is >> tmp;

		//load basic cube
		rect.loadMesh(tmp.c_str());
	}
	else if(ele == "h")//hole
	{
		double x, z, r;
		is >> x >> z >> r;
		setHole(x,z,r);
	}
	else if(ele == "w")//wall
	{
		double x, z, sx, sz;
		is >> x >> z >> sx >> sz;
		setWall(x, z, sx, sz);
	}
	else if(ele == "b")//begin
	{
		std::string tmp;

		//get filename
		is >> tmp;

		//load obj
		begin.loadMesh(tmp.c_str());

		//set color to green
		begin.setColor(0.0,1.0,0.0);

		//append mesh
		mesh.addMesh(begin.getMesh());
	}
	else if(ele == "g")//goal
	{
		std::string tmp;

		//get filename
		is >> tmp;

		//load obj
		goal.loadMesh(tmp.c_str());

		//set color to red
		goal.setColor(1.0,0.0,0.0);

		//append mesh
		mesh.addMesh(goal.getMesh());
	}
	else
	{
		//skip anything else
		skipLine(is);
	}
	return true;
}