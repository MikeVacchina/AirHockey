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
	theda=0.0;
	phi=50.0;
	
    ai_enabled = false;
    paddle_sensitivity = 200*25;
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
	
	const GLubyte *vers = glGetString(GL_VERSION);

	std::cout << "OpenGL Version " << vers << " Detected.\n";

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

    //create menus
	createMenus();
	
    //load objects for game
	if(!display.loadObjects())
		return false;
	
	//get references to objects in order to match global defines
	objs.push_back(display.getPuckReference());
	objs.push_back(display.getPaddle1Reference());
	objs.push_back(display.getPaddle2Reference());
	objs.push_back(display.getTableReference());

	//set object references in physics
	physics.setObjs(objs);
	
	//set object references in collision
	collision.setPuck(display.getPuckReference());
	collision.setPaddle1(display.getPaddle1Reference());
	collision.setPaddle2(display.getPaddle2Reference());
	collision.setTable(display.getTableReference());

    aiInput.setPuck(display.getPuckReference());
	aiInput.setPaddle2(display.getPaddle2Reference());

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

void framework_AirHockey::createMenus()
{
	//create settings menu
	settingsMenu = glutCreateMenu(subMenuWrapperFunc);
	
	//set settings menu options
	glutAddMenuEntry("Toggle AI", TOGGLE_AI);
    glutAddMenuEntry("Toggle Light 1", TOGGLE_LIGHT_1);
	glutAddMenuEntry("Toggle Light 2", TOGGLE_LIGHT_2);
   	glutAddMenuEntry("Increase Paddle Sensitivity", INCREASE_PADDLE_SENSITIVITY);
    glutAddMenuEntry("Decrease Paddle Sensitivity", DECREASE_PADDLE_SENSITIVITY);
	//create main menu
	menu = glutCreateMenu(menuWrapperFunc);
	
	//set main menu options
    glutAddMenuEntry("Restart", RESTART);
	glutAddMenuEntry("Reset Paddles and Puck", RESET);
	glutAddSubMenu("Settings", settingsMenu);
	glutAddMenuEntry("Quit", QUIT);

	//attack main menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	glm::mat4 modelOffset;

	double xVelocity=0.0, zVelocity=0.0;
	double deltaThedaTime=0.0;
	double deltaPhiTime=0.0;
	double keyRotationRate = 30.0;

	//Update camera position from input

	deltaThedaTime -= userInput.timeKeyDown('l');
	deltaThedaTime += userInput.timeKeyDown('j');

	deltaPhiTime -= userInput.timeKeyDown('k');
	deltaPhiTime += userInput.timeKeyDown('i');
	
	phi = phi + deltaPhiTime * keyRotationRate;
	theda = theda + deltaThedaTime * keyRotationRate;

	//constrain max and min positions
	if(phi < 0)
		phi = 0.0;
	if(phi > 85.0)
		phi = 85.0;
	
	glm::mat4 rotationTheda = glm::rotate(glm::mat4(1.0f), (float)theda, glm::vec3(0.0,1.0,0.0));

	glm::vec4 phiAxis = glm::vec4(1.0,0.0,0.0,0.0);

	phiAxis = rotationTheda * phiAxis;
	
	glm::mat4 rotationPhi = glm::rotate(glm::mat4(1.0f), (float)phi, glm::vec3(phiAxis.x, phiAxis.y, phiAxis.z));

	glm::vec4 camPos = glm::vec4(0.0,0.0,-30.0,0.0);

	camPos = rotationPhi*rotationTheda*camPos;

	display.setCamPos(glm::vec3(camPos.x,camPos.y,camPos.z));

	//update object velocity from input

	//get time of each key down as there effect on paddle1's position
	xVelocity -= userInput.timeSpecialDown(GLUT_KEY_RIGHT);
	xVelocity += userInput.timeSpecialDown(GLUT_KEY_LEFT);

	zVelocity -= userInput.timeSpecialDown(GLUT_KEY_DOWN);
	zVelocity += userInput.timeSpecialDown(GLUT_KEY_UP);

	xVelocity *= paddle_sensitivity;
	zVelocity *= paddle_sensitivity;

	objs[PADDLE1]->vel = objs[PADDLE1]->vel+glm::vec3(xVelocity, 0.0, zVelocity);
	
	xVelocity=0.0;
	zVelocity=0.0;

    if (ai_enabled)
    {
        xVelocity += aiInput.updateXVelocity();
	    //zVelocity += aiInput.updateZVelocity();

	    xVelocity *= paddle_sensitivity;
	    //zVelocity *= paddle_sensitivity;
    }
    else
    {
        //get time of each key down as there effect on paddle2's position
	    xVelocity -= userInput.timeKeyDown('d');
	    xVelocity += userInput.timeKeyDown('a');

	    zVelocity -= userInput.timeKeyDown('s');
	    zVelocity += userInput.timeKeyDown('w');
	
	    xVelocity *= paddle_sensitivity;
	    zVelocity *= paddle_sensitivity;
    }
	
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
	
	//handle goals
	if(code == GOAL1)
    {
        display.addScore(1);
		std::cout << "Goal Scored 1\n";
        objs[PUCK]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PUCK]->pos = glm::vec3(0.0,0.0,3.0);
        objs[PADDLE1]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->pos = glm::vec3(0.0,0.0,-5.5);
        objs[PADDLE2]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE2]->pos = glm::vec3(0.0,0.0,5.5);
    }
	if(code == GOAL2)
	{
        display.addScore(2);
        std::cout << "Goal Scored 2\n";
        objs[PUCK]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PUCK]->pos = glm::vec3(0.0,0.0,-3.0);
        objs[PADDLE1]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->pos = glm::vec3(0.0,0.0,-5.5);
        objs[PADDLE2]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE2]->pos = glm::vec3(0.0,0.0,5.5);
    }
    if (display.checkForWinner ())
    {
        if (display.checkForWinner () == 1)
        {
            objs[PUCK]->vel = glm::vec3(0.0,0.0,0.0);
            objs[PUCK]->pos = glm::vec3(0.0,0.0,3.0);
        }
        else
        {
            objs[PUCK]->vel = glm::vec3(0.0,0.0,0.0);
            objs[PUCK]->pos = glm::vec3(0.0,0.0,-3.0);
        }
        objs[PADDLE1]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->pos = glm::vec3(0.0,0.0,-5.5);
        objs[PADDLE2]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE2]->pos = glm::vec3(0.0,0.0,5.5);
        display.resetScore();
    }

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


void framework_AirHockey::menuFunc(int option)
{
	mvMaze *m;
	mvSphere *b;

	switch(option)
	{
	case RESTART:
        objs[PUCK]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PUCK]->pos = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->pos = glm::vec3(0.0,0.0,-5.5);
        objs[PADDLE2]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE2]->pos = glm::vec3(0.0,0.0,5.5);
        display.resetScore();
		break;
	case RESET:
        objs[PUCK]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PUCK]->pos = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE1]->pos = glm::vec3(0.0,0.0,-5.5);
        objs[PADDLE2]->vel = glm::vec3(0.0,0.0,0.0);
        objs[PADDLE2]->pos = glm::vec3(0.0,0.0,5.5);
		break;
	case QUIT:
		//quit
		exit(0);
		break;
	}
}

void framework_AirHockey::subMenuFunc(int option)
{
	switch(option)
	{
    case TOGGLE_AI:
        ai_enabled = ai_enabled?false:true;
        break;
    case TOGGLE_LIGHT_1:
        display.toggleLightOne();
        break;
    case TOGGLE_LIGHT_2:
        display.toggleLightTwo();
        break;
    case INCREASE_PADDLE_SENSITIVITY:
		paddle_sensitivity*=1.5;
        break;
    case DECREASE_PADDLE_SENSITIVITY:
		paddle_sensitivity/=1.5;
        break;
	}
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

void menuWrapperFunc(int option)
{
	framework_AirHockey::instance()->menuFunc(option);
}

void subMenuWrapperFunc(int option)
{
	framework_AirHockey::instance()->subMenuFunc(option);
}