#include "assimpObjLoader.h"

assimpObjLoader::assimpObjLoader()
{
}

assimpObjLoader::~assimpObjLoader()
{
}

//TODO: Potential Bug
//when loading uv's it assumes there is only 1 set of uv's and it is the first set
bool assimpObjLoader::loadFile(const char *filename)
{
	Assimp::Importer importer;

	mesh.clear();

	const aiScene *scene = importer.ReadFile(filename,aiProcess_Triangulate | aiProcess_GenNormals );

	if(!scene)
	{
		return false;
	}
	
	aiFace *faces = (*scene->mMeshes)->mFaces;
	aiVector3D *vertices = (*scene->mMeshes)->mVertices;
	aiVector3D *vertexNormals = (*scene->mMeshes)->mNormals;
	aiVector3D *vertexUV = NULL;
	bool textured = (*scene->mMeshes)->HasTextureCoords(0);
	if(textured)
		vertexUV = (*scene->mMeshes)->mTextureCoords[0];
	
	int faceCount = (*scene->mMeshes)->mNumFaces;
	int vertexCount = (*scene->mMeshes)->mNumVertices;

	for(int i=0;i<vertexCount;++i)
	{
		mesh.addVertex(vec3(vertices[i].x, vertices[i].y, vertices[i].z));
		mesh.addVertexNorm(vec3(vertexNormals[i].x, vertexNormals[i].y, vertexNormals[i].z));
		if(textured)
			mesh.addUV(vec2(vertexUV[i].x, vertexUV[i].y));
	}

	for( int i=0;i<faceCount;++i)
	{
		mesh.addFace(vec3(faces[i].mIndices[0], faces[i].mIndices[1], faces[i].mIndices[2]));
	}

	return true;
}

mvMesh assimpObjLoader::getMesh()
{
	return mesh;
}