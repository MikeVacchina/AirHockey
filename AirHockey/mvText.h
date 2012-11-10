#ifndef MVTEXT
#define MVTEXT

#include <stdio.h>    //  Standard Input\Output C Library
#include <stdarg.h>   //  To use functions with variables arguments
#include <stdlib.h>   //  for malloc
#include <gl/glut.h>  //  Include GLUT, OpenGL, and GLU libraries

class mvText
{
public:
	mvText();
	~mvText();

	void printw (float x, float y, float z, char* format, ...);

private:
	GLvoid *font_style;
};

#endif //MVTEXT