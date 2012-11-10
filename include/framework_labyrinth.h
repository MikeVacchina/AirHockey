#ifndef FRAMEWORK_LABYRINTH
#define FRAMEWORK_LABYRINTH

#include <Windows.h>
#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "defines.h"

#include "mvSimpleStructs.h"

#include "mvShader.h"
#include "mvInput.h"
#include "mvTime.h"
#include "mvDisplay.h"
#include "mvPhysics.h"
#include "mvCollision.h"
#include "mvObject.h"
#include "mvSphere.h"
#include "mvMaze.h"

//M_PI does not appear to be defined when I build the project in visual studios
#define M_PI        3.14159265358979323846264338327950288   /* pi */

//Initialize/Start Glut Functions (wrappers basically)
extern void initializeGlut(int argc, char **argv);
extern void startGlut();

//framework class for labyrinth game
//singleton class
//accessed by framework_labyrith::instance()
class framework_labyrinth
{
public:
	//singleton instance
	static framework_labyrinth* instance();

	//initialize labyrinth with glew/glut
	bool initialize(std::string windowName, int windowWdith, int windowHeight);

private:
	//make constructor private for singleton class
	framework_labyrinth();

	void initializeCallbacks();

	void createMenus();

	//framework function callbacks
	void displayFunc();
	void reshapeFunc(int newWidth, int newHeight);
	void keyboardFunc(unsigned char key, int x, int y);
	void keyboardUpFunc(unsigned char key, int x, int y);
	void specialFunc(int key, int x, int y);
	void specialUpFunc(int key, int x, int y);
	void mouseFunc(int button, int state, int x, int y);
	void motionFunc(int x, int y);
	void idleFunc();
	void menuFunc(int option);
	void subMenuFunc(int option);

	//variable setters to enforce constraints on variables
	void setTheda(double t);
	void setPhi(double p);

	//timer
	mvTime stopwatch;

	//display handler
	mvDisplay display;

	//input handler
	mvInput userInput;

	//physics handler
	mvPhysics physics;

	//collision handler
	mvCollision collision;
	
	//menu ids
	int menu;
	int settingsMenu;

	bool started;

	//keep a reference to objects in framework
	std::vector<mvObject*> objs;
	
	//maze orientation variables
	double theda, phi;
	double keyRotationRate;
	double gravityVar;
	
	//singleton instance
	static framework_labyrinth *__framework_labyrinth__;

	//friend callbacks for glut
	friend void displayWrapperFunc();
	friend void reshapeWrapperFunc(int newWidth, int newHeight);
	friend void keyboardWrapperFunc(unsigned char key, int x, int y);
	friend void keyboardUpWrapperFunc(unsigned char key, int x, int y);
	friend void specialWrapperFunc(int key, int x, int y);
	friend void specialUpWrapperFunc(int key, int x, int y);
	friend void mouseWrapperFunc(int button, int state, int x, int y);
	friend void motionWrapperFunc(int x, int y);
	friend void idleWrapperFunc();
	friend void menuWrapperFunc(int option);
	friend void subMenuWrapperFunc(int option);
};

//callbacks for glut which call framework funcs (wrappers)
extern void displayWrapperFunc();
extern void reshapeWrapperFunc(int newWidth, int newHeight);
extern void keyboardWrapperFunc(unsigned char key, int x, int y);
extern void keyboardUpWrapperFunc(unsigned char key, int x, int y);
extern void specialWrapperFunc(int key, int x, int y);
extern void specialUpWrapperFunc(int key, int x, int y);
extern void mouseWrapperFunc(int button, int state, int x, int y);
extern void motionWrapperFunc(int x, int y);
extern void idleWrapperFunc();
extern void menuWrapperFunc(int option);
extern void subMenuWrapperFunc(int option);

#endif //FRAMEWORK_LABYRINTH