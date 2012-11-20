#include "framework_AirHockey.h"

//Singleton class get initialized to NULL
framework_AirHockey* framework_AirHockey::__framework_AirHockey__ = NULL;

extern void initializeGlut(int argc, char **argv)
{
    glutInit(&argc, argv);
}

extern void startGlut()
{
	glutMainLoop();
}

framework_AirHockey::framework_AirHockey()
{
	//set default values
}

framework_AirHockey* framework_AirHockey::instance()
{
	//check if object already created
	if(!__framework_AirHockey__)
		__framework_AirHockey__ = new framework_AirHockey();
	return __framework_AirHockey__;
}

bool framework_AirHockey::initialize(std::string windowName, int windowWidth, int windowHeight)
{
	//initialize display size and window name
	display.initializeDisplay(windowName, windowWidth, windowHeight);

	// Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[ERROR] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return false;
    }

	//set key repeat to be ignored for smooth key presses
	glutIgnoreKeyRepeat(true);
	
	//initialize callbacks
	initializeCallbacks();

	//load objects for game
	if(!display.loadObjects())
		return false;
	
	//get references to objects in order to match global defines
	objs.push_back(display.getPuckReference());
	objs.push_back(display.getPaddle1Reference());
	objs.push_back(display.getPaddle2Reference());
	objs.push_back(display.getTableReference());

	physics.setObjs(objs);
	
	collision.setPuck(display.getPuckReference());
	collision.setPaddle1(display.getPaddle1Reference());
	collision.setPaddle2(display.getPaddle2Reference());
	collision.setTable(display.getTableReference());

	//collision.bouncyness = 1.0;

	//initialize resources
	if(!display.initializeDisplayResources())
		return false;

	//start stopwatch
	stopwatch.startTime();

	return true;
}

void framework_AirHockey::initializeCallbacks()
{
	//set callbacks to friended functions
	glutDisplayFunc(displayWrapperFunc);
	glutReshapeFunc(reshapeWrapperFunc);
	glutKeyboardFunc(keyboardWrapperFunc);
	glutKeyboardUpFunc(keyboardUpWrapperFunc);
	glutSpecialFunc(specialWrapperFunc);
	glutSpecialUpFunc(specialUpWrapperFunc);
	glutMouseFunc(mouseWrapperFunc);
	glutMotionFunc(motionWrapperFunc);
	glutIdleFunc(idleWrapperFunc);
}

void framework_AirHockey::displayFunc()
{
	//pass to display
	display.display();
}

void framework_AirHockey::reshapeFunc(int newWidth, int newHeight)
{
	//tell display to reshape
	display.reshape(newWidth, newHeight);
}

void framework_AirHockey::keyboardFunc(unsigned char key, int x, int y)
{
	//pass to input
	userInput.handleKeyboardFunc(key,mvKeyboardData(KEY_DOWN));
}

void framework_AirHockey::keyboardUpFunc(unsigned char key, int x, int y)
{
	//pass to input
	userInput.handleKeyboardFunc(key,mvKeyboardData(KEY_UP));
}

void framework_AirHockey::specialFunc(int key, int x, int y)
{
	//pass to input
	userInput.handleSpecialFunc(key,mvKeyboardData(KEY_DOWN));
}

void framework_AirHockey::specialUpFunc(int key, int x, int y)
{
	//pass to input
	userInput.handleSpecialFunc(key,mvKeyboardData(KEY_UP));
}

void framework_AirHockey::mouseFunc(int button, int state, int x, int y)
{
	//pass to input
	userInput.handleMouseFunc(button, state, x, y, mvMouseData(objs[PADDLE1]->vel));
}

void framework_AirHockey::motionFunc(int x, int y)
{
	//pass to input
	mvMouseData mouseOutput;
	if(userInput.handleMouseMotionFunc(x, y, mouseOutput))
	{
		//set values based on mouse movements
		objs[PADDLE1]->vel = mouseOutput.vel;
	}
}

void framework_AirHockey::idleFunc()
{
	//update object velocity from input
	
	glm::mat4 modelOffset;

	double xVelocity=0.0, zVelocity=0.0;
	double modifier = 200.0;
	
	//get time of each key down as there effect on paddle1's position
	xVelocity -= userInput.timeSpecialDown(GLUT_KEY_RIGHT);
	xVelocity += userInput.timeSpecialDown(GLUT_KEY_LEFT);

	zVelocity -= userInput.timeSpecialDown(GLUT_KEY_DOWN);
	zVelocity += userInput.timeSpecialDown(GLUT_KEY_UP);
	
	xVelocity *= modifier;
	zVelocity *= modifier;

	objs[PADDLE1]->vel = objs[PADDLE1]->vel+glm::vec3(xVelocity, 0.0, zVelocity);
	
	xVelocity=0.0;
	zVelocity=0.0;

	//get time of each key down as there effect on paddle2's position
	xVelocity -= userInput.timeKeyDown('d');
	xVelocity += userInput.timeKeyDown('a');

	zVelocity -= userInput.timeKeyDown('s');
	zVelocity += userInput.timeKeyDown('w');
	
	xVelocity *= modifier;
	zVelocity *= modifier;
	
	objs[PADDLE2]->vel = objs[PADDLE2]->vel+glm::vec3(xVelocity, 0.0, zVelocity);
	
	double len = glm::length(objs[PADDLE1]->vel);
	if(len>15)
	{
		double scale = 15.0/len;

		objs[PADDLE1]->vel = objs[PADDLE1]->vel * glm::vec3(scale);
	}
	len = glm::length(objs[PADDLE2]->vel);
	if(len>15)
	{
		double scale = 15.0/len;

		objs[PADDLE2]->vel = objs[PADDLE2]->vel * glm::vec3(scale);
	}
		
	//update physics for objects
	physics.update(stopwatch.resetTime());
	
	//resolve any possible collisions that might occur
	int code = collision.resolveCollisions();
	
	if(code == GOAL1)
		std::cout << "Goal Scored 1\n";
	if(code == GOAL2)
		std::cout << "Goal Scored 2\n";

	float angle = 90.0;
	glm::mat4 r = glm::rotate(glm::mat4(1.0f),angle,glm::vec3(1.0f,0.0f,0.0f));
	
	//set new puck position
	objs[PUCK]->model = glm::translate(glm::mat4(1.0f),objs[PUCK]->pos)*r;
	//set new paddle1 position
	objs[PADDLE1]->model = glm::translate(glm::mat4(1.0f),objs[PADDLE1]->pos)*r;
	//set new paddle2 position
	objs[PADDLE2]->model = glm::translate(glm::mat4(1.0f),objs[PADDLE2]->pos)*r;

	objs[TABLE]->model = r;
	
	objs[PADDLE1]->vel = glm::vec3(0.0);
	objs[PADDLE2]->vel = glm::vec3(0.0);

	//update display
	display.display();

	//call the glut display callback
    glutPostRedisplay();
}

//glut callback wrapper functions simply call corresponding functions in framework as glut is a c api
void displayWrapperFunc()
{
	framework_AirHockey::instance()->displayFunc();
}

void reshapeWrapperFunc(int newWidth, int newHeight)
{
	framework_AirHockey::instance()->reshapeFunc(newWidth, newHeight);
}

void keyboardWrapperFunc(unsigned char key, int x, int y)
{
	framework_AirHockey::instance()->keyboardFunc(key, x, y);
}

void keyboardUpWrapperFunc(unsigned char key, int x, int y)
{
	framework_AirHockey::instance()->keyboardUpFunc(key, x, y);
}

void specialWrapperFunc(int key, int x, int y)
{
	framework_AirHockey::instance()->specialFunc(key, x, y);
}

void specialUpWrapperFunc(int key, int x, int y)
{
	framework_AirHockey::instance()->specialUpFunc(key, x, y);
}

void mouseWrapperFunc(int button, int state, int x, int y)
{
	framework_AirHockey::instance()->mouseFunc(button, state, x, y);
}

void motionWrapperFunc(int x, int y)
{
	framework_AirHockey::instance()->motionFunc(x, y);
}

void idleWrapperFunc()
{
	framework_AirHockey::instance()->idleFunc();
}