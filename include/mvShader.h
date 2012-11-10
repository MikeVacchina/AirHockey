#ifndef MVSHADER
#define MVSHADER

#include <string>
#include <fstream>

//shader loading class
class mvShader
{
public:
	mvShader(char *filename = NULL);
	~mvShader();

	//load shader from file
	void loadShader(char* filename);
	
	//return shader as const char * for glut
	const char* getShaderString();

	//check if mvShader has loaded a valid shader 
	bool isShaderRdy();

	//get error string
	std::string getError();

private:
	//helper function to get size of data in file
	unsigned long mvShader::getFileLength(std::ifstream& file);

	std::string shader;
	bool shaderRdy;
	std::string error;
};

#endif //MVSHADER