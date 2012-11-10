#ifndef ASSIMPOBJLOADER
#define ASSIMPOBJLOADER

#include <vector>

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "mvMesh.h"

class assimpObjLoader
{
public:
	assimpObjLoader();
	~assimpObjLoader();

	bool loadFile(const char *filename);
	mvMesh getMesh();

	mvMesh mesh;
};

#endif