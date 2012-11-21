#include "mvObject.h"


mvObject::mvObject()
{
	//set default values
	geometry = NULL;
	vertexCount = 0;

	textured = false;
	
	acc = glm::vec3(0.0);
	vel = glm::vec3(0.0);
	pos = glm::vec3(0.0);

	mass = 1.0;
}

mvObject::~mvObject()
{
	//TODO: fix: seems to be a bug with this
	//if(geometry && vertexCount > 1)
	//	delete [] geometry;
	//else if(geometry && vertexCount > 0)
	//	delete geometry;
}

void mvObject::loadMesh(const char *filename)
{
	std::string error;
	
	//load mesh
	mvObjLoader(filename, mesh, error);

	//check for error
	if(error != "")
	{
        std::cerr << "[ERROR] " << filename << " FAILED TO LOAD";
		std::cerr << error;
		return;
	}
	
	//set geometry and vertex count based on mesh
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
	textured = mesh.hasTexture();
	if(textured)
	{
		//set color for entire mesh
		mesh.setColor(-1, -1, -1);
		//reload geometry
		geometry = mesh.getGeometry();

		textureID = mesh.getTextureID();
	}
}

void mvObject::assimpLoadMesh(const char *filename)
{
	assimpObjLoader aol;

	aol.loadFile(filename);

	mesh = aol.getMesh();

	if(mesh.hasTexture())
	{
		textured = true;
		//set color for entire mesh
		mesh.setColor(-1, -1, -1);
	}

	//set geometry and vertex count based on mesh
	geometry = mesh.getGeometry();
	vertexCount = mesh.countVertices();
}

bool mvObject::loadBmpTexture(const char *filename)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char *data;

	FILE * file = fopen(filename,"rb");
	if (!file)
	{
		//file loading failed
		return false;
	}

	if ( fread(header, 1, 54, file)!=54 )
		{ // If not 54 bytes read : problem
		//printf("Not a correct BMP file\n");
		return false;
	}

	if ( header[0]!='B' || header[1]!='M' )
	{
		//printf("Not a correct BMP file\n");
		return false;
	}
	
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )             
		return false;
	if ( *(int*)&(header[0x1C])!=24 )         
		return false;
	
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

	//textureID=0;
	// Create one OpenGL texture
	glGenTextures(1, &textureID);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
 
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

mvMesh mvObject::getMesh()
{
	//return copy of mesh
	return mesh;
}

void mvObject::setColor(double r, double g, double b)
{
	if(!textured)
	{
		//set color for entire mesh
		mesh.setColor(r, g, b);
		//reload geometry
		geometry = mesh.getGeometry();
	}
}

bool mvObject::hasTexture()
{
	return textured;
}

int mvObject::sizeofgeometry()
{
	//return size of geometry
	return vertexCount*sizeof(mvVertex);
}

void mvObject::bind()
{
	if(textured)
	{
		//glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
}