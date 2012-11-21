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

	//load the file
	const aiScene *scene = importer.ReadFile(filename,aiProcess_Triangulate | aiProcess_GenNormals );

	if(!scene)
	{
		return false;
	}
	
	//get faces from assimp
	aiFace *faces = (*scene->mMeshes)->mFaces;
	//get vertices from assimp
	aiVector3D *vertices = (*scene->mMeshes)->mVertices;
	//get normals from assimp
	aiVector3D *vertexNormals = (*scene->mMeshes)->mNormals;
	//get testures if avalable from assimp
	aiVector3D *vertexUV = NULL;
	bool textured = (*scene->mMeshes)->HasTextureCoords(0);
	if(textured)
		vertexUV = (*scene->mMeshes)->mTextureCoords[0];
	
	//get face and vertex count
	int faceCount = (*scene->mMeshes)->mNumFaces;
	int vertexCount = (*scene->mMeshes)->mNumVertices;

	//add vertex, normals, and UVs to mesh object
	for(int i=0;i<vertexCount;++i)
	{
		mesh.addVertex(vec3(vertices[i].x, vertices[i].y, vertices[i].z));
		mesh.addVertexNorm(vec3(vertexNormals[i].x, vertexNormals[i].y, vertexNormals[i].z));
		if(textured)
			mesh.addUV(vec2(vertexUV[i].x, vertexUV[i].y));
	}

	//add faces to mesh object
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