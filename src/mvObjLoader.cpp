#include "mvObjLoader.h"


mvObjLoader::mvObjLoader()
{
}


mvObjLoader::~mvObjLoader()
{
}

mvObjLoader::mvObjLoader(std::string filename, mvMesh &mesh, std::string &error)
{
	//redirect
	loadObj(filename, mesh, error);
}

mvObjLoader::mvObjLoader(char *filename, mvMesh &mesh, char *error)
{
	//redirect
	loadObj(filename, mesh, error);
}
	
void mvObjLoader::loadObj(std::string filename, mvMesh &mesh, std::string &error)
{
	//convert filename to cstring
	char *e=NULL;
	char *fn=new char[filename.size()+1];
	fn[filename.size()+1] = NULL;

	std::strcpy(fn,filename.c_str());

	//redirect
	loadObj(fn, mesh, e);

	if(e)//if error convert to string
		error = std::string(e);
}

void mvObjLoader::loadObj(char *filename, mvMesh &mesh, char *error)
{
	//clear previous mesh
	mesh.clear();

	//open file
	std::ifstream ifs(filename, std::ios::in);

	//check for error
	if(!ifs)
	{
		error = "[ERROR] CANNOT OPEN FILE!";
		return;
	}

	//load obj
	while(skipCommentLine(ifs))
	{
		if(!processLine(mesh,ifs))
			break;
	}
}

bool mvObjLoader::skipCommentLine(std::istream& is)
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

void mvObjLoader::skipLine(std::istream &is)
{
	char next;

	//do not skip white space
	is >> std::noskipws;

	//load line and ignore all of it
	while( (is>>next) && (next != '\n') );
}
	
bool mvObjLoader::processLine(mvMesh &mesh, std::istream &is)
{
	std::string ele;
	double x, y, z;

	//check for eof
	if( !(is >> ele) )
		return false;

	if(ele == "mtllib")
	{
		//nothing to do here...
		skipLine(is);
	}
	else if(ele == "usemtl")
	{
		//we are going to cheat a bit and put texture file here
		is >> ele;
		mesh.addTexture(ele);
	}
	else if(ele == "v")
	{
		//add vertex
		is >> x >> y >> z;
		mesh.addVertex( vec3(x, y, z) );
	}
	else if(ele == "vt")
	{
		is >> x >> y;
		mesh.addUV( vec2(x, y) );
	}
	else if(ele == "vn")
	{
		is >> x >> y >> z;
		mesh.addVertexNorm( vec3(x, y, z) );
	}
	else if(ele == "f")
	{
		//assume for now only triangles with only vertices
		//add face
		is >> x >> y >> z;
		mesh.addFace( vec3(x-1, y-1, z-1) );
	}
	else
	{
		//skip anything else
		skipLine(is);
	}
	return true;
}