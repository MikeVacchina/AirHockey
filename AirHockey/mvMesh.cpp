#include "mvMesh.h"

mvMesh::mvMesh()
{
}

mvMesh::~mvMesh()
{
}

void mvMesh::addVertex(vec3 v)
{
	vertices.push_back(v);
}

void mvMesh::addFace(vec3 f)
{
	faces.push_back(f);
}

void mvMesh::addVertexNorm(vec3 vn)
{
	vertexNorms.push_back(vn);
}

void mvMesh::addUV(vec2 vt)
{
	UV.push_back(vt);
}

void mvMesh::addTexture(std::string filename)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char *data;

	FILE * file = fopen(filename.c_str(),"rb");
	if (!file)
	{
		//file loading failed
		return;
	}

	if ( fread(header, 1, 54, file)!=54 )
		{ // If not 54 bytes read : problem
		//printf("Not a correct BMP file\n");
		return;
	}

	if ( header[0]!='B' || header[1]!='M' )
	{
		//printf("Not a correct BMP file\n");
		return;
	}
	
	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    
		imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      
		dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];
 
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
 
	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &textureID);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

bool mvMesh::hasTexture()
{
	return UV.size()>0;
}

GLuint mvMesh::getTextureID()
{
	if(hasTexture())
		return textureID;
	return 0;
}

void mvMesh::setColor(vec3 c)
{
	//clear color
	colors.clear();

	//set color as a single colored object
	colors.push_back(c);
}

void mvMesh::setColor(double r, double g, double b)
{
	//clear color
	colors.clear();

	//set color as a single colored object
	colors.push_back(vec3(r, g, b));
}

mvVertex* mvMesh::getGeometry()
{
	mvVertex *rtn = new mvVertex[faces.size()*3];

	//check if single color or not
	bool singleColor = colors.size()==vertices.size() ? false : true;

	//if no color add default color of black
	if( (singleColor && colors.size()==0) )
		colors.push_back(vec3(0.0,0.0,0.0));

	//loop through faces creating each face
	for(int i=0,size=faces.size();i<size;++i)
	{
		int j=0;

		//set first vertex in face
		rtn[i*3+j].position[0] = float(vertices[int(faces[i].x)].x);
		rtn[i*3+j].position[1] = float(vertices[int(faces[i].x)].y);
		rtn[i*3+j].position[2] = float(vertices[int(faces[i].x)].z);

		//set first vertex's normal in face
		rtn[i*3+j].normal[0] = float(vertexNorms[int(faces[i].x)].x);
		rtn[i*3+j].normal[1] = float(vertexNorms[int(faces[i].x)].y);
		rtn[i*3+j].normal[2] = float(vertexNorms[int(faces[i].x)].z);
		
		if(hasTexture())
		{
			rtn[i*3+j].uv[0] = float(UV[int(faces[i].x)].x);
			rtn[i*3+j].uv[1] = float(UV[int(faces[i].x)].y);
		}

		if(singleColor)
		{
			rtn[i*3+j].color[0] = float(colors[0].x);
			rtn[i*3+j].color[1] = float(colors[0].y);
			rtn[i*3+j].color[2] = float(colors[0].z);
		}
		else
		{
			rtn[i*3+j].color[0] = float(colors[int(faces[i].x)].x);
			rtn[i*3+j].color[1] = float(colors[int(faces[i].x)].y);
			rtn[i*3+j].color[2] = float(colors[int(faces[i].x)].z);
		}

		++j;
		
		//set second vertex in face
		rtn[i*3+j].position[0] = float(vertices[int(faces[i].y)].x);
		rtn[i*3+j].position[1] = float(vertices[int(faces[i].y)].y);
		rtn[i*3+j].position[2] = float(vertices[int(faces[i].y)].z);

		//set first vertex's normal in face
		rtn[i*3+j].normal[0] = float(vertexNorms[int(faces[i].y)].x);
		rtn[i*3+j].normal[1] = float(vertexNorms[int(faces[i].y)].y);
		rtn[i*3+j].normal[2] = float(vertexNorms[int(faces[i].y)].z);
		
		if(hasTexture())
		{
			rtn[i*3+j].uv[0] = float(UV[int(faces[i].y)].x);
			rtn[i*3+j].uv[1] = float(UV[int(faces[i].y)].y);
		}

		if(singleColor)
		{
			rtn[i*3+j].color[0] = float(colors[0].x);
			rtn[i*3+j].color[1] = float(colors[0].y);
			rtn[i*3+j].color[2] = float(colors[0].z);
		}
		else
		{
			rtn[i*3+j].color[0] = float(colors[int(faces[i].y)].x);
			rtn[i*3+j].color[1] = float(colors[int(faces[i].y)].y);
			rtn[i*3+j].color[2] = float(colors[int(faces[i].y)].z);
		}

		++j;
		
		//set third vertex in face
		rtn[i*3+j].position[0] = float(vertices[int(faces[i].z)].x);
		rtn[i*3+j].position[1] = float(vertices[int(faces[i].z)].y);
		rtn[i*3+j].position[2] = float(vertices[int(faces[i].z)].z);

		//set first vertex's normal in face
		rtn[i*3+j].normal[0] = float(vertexNorms[int(faces[i].z)].x);
		rtn[i*3+j].normal[1] = float(vertexNorms[int(faces[i].z)].y);
		rtn[i*3+j].normal[2] = float(vertexNorms[int(faces[i].z)].z);
		
		if(hasTexture())
		{
			rtn[i*3+j].uv[0] = float(UV[int(faces[i].z)].x);
			rtn[i*3+j].uv[1] = float(UV[int(faces[i].z)].y);
		}

		if(singleColor)
		{
			rtn[i*3+j].color[0] = float(colors[0].x);
			rtn[i*3+j].color[1] = float(colors[0].y);
			rtn[i*3+j].color[2] = float(colors[0].z);
		}
		else
		{
			rtn[i*3+j].color[0] = float(colors[int(faces[i].z)].x);
			rtn[i*3+j].color[1] = float(colors[int(faces[i].z)].y);
			rtn[i*3+j].color[2] = float(colors[int(faces[i].z)].z);
		}
	}

	return rtn;
}

int mvMesh::countVertices()
{
	//return vertex count
	return faces.size()*3;
}

void mvMesh::addMesh(mvMesh &mesh)
{
	int origSize = (int)vertices.size();
	
	//check if single color or not
	bool singleColor = colors.size()==vertices.size() ? false : true;
	
	//if no color add default color of black
	if( (singleColor && colors.size()==0) )
		colors.push_back(vec3(0.0,0.0,0.0));

	//if single color make multi color of same color
	if(singleColor)
	{
		vec3 tmpColor = colors[0];
		colors.clear();

		for(int i=0,size=vertices.size();i<size;++i)
			colors.push_back(tmpColor);
	}

	//check if single color or not of mesh being added
	singleColor = mesh.colors.size()==mesh.vertices.size() ? false : true;
	
	//if no color add default color of black
	if( (singleColor && mesh.colors.size()==0) )
		mesh.colors.push_back(vec3());

	//add color and vertices to current list of color and vertices
	for(int i=0,size=mesh.vertices.size();i<size;++i)
	{
		vertices.push_back(mesh.vertices[i]);
		vertexNorms.push_back(mesh.vertexNorms[i]);

		if(singleColor)
			colors.push_back(mesh.colors[0]);
		else
			colors.push_back(mesh.colors[i]);
	}

	//add faces to current list of faces
	for(int i=0,size=mesh.faces.size();i<size;++i)
	{
		faces.push_back(mesh.faces[i].plus(origSize));
	}
}

void mvMesh::scale(double s)
{
	//scale mesh
	for(int i=0,size=vertices.size();i<size;++i)
		vertices[i] = vertices[i].mulScalar(s);
}

void mvMesh::scale(vec3 s)
{
	//scale mesh
	for(int i=0,size=vertices.size();i<size;++i)
		vertices[i] = vertices[i].mul(s);
}

void mvMesh::translate(vec3 v)
{
	//translate mesh
	for(int i=0,size=vertices.size();i<size;++i)
		vertices[i] = vertices[i].plus(v);
}

void mvMesh::clear()
{
	//clear mesh data
	vertices.clear();
	faces.clear();
	vertexNorms.clear();
	colors.clear();
	UV.clear();
}