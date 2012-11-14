#ifndef MVINPUT
#define MVINPUT

#include <map>

#include <Windows.h>
#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "mvKey.h"

#include "mvSimpleStructs.h"

//this class handles user input throught the mouse and keyboard
class mvInput
{
public:
	mvInput();
	~mvInput();

	//mouse callback
	void handleMouseFunc(int button, int state, int x, int y, mvMouseData mouseInput);

	//mouse motion callback
	bool handleMouseMotionFunc(int x, int y, mvMouseData &mouseOutput);
	
	//keyboard callback
	void handleKeyboardFunc(unsigned char key, mvKeyboardData keyboardInput);

	//special key (arrows) callback
	void handleSpecialFunc(int key, mvKeyboardData keyboardInput);
	
	//return key down time
	double timeKeyDown(unsigned char key);

	//return special key down time
	double timeSpecialDown(int key);
	
	//change mouse sensitivity
	void increaseMouseSensitivity();
	void decreaseMouseSensitivity();

	//reset mouse sensitivity to default
	void resetMouseSensitivity();

private:
	double init_x, init_y;
	glm::vec3 init_pos;

	_mouseState lMouse;
	_mouseState rMouse;

	double mouseSensitivity;
	
	//store each key state in a map
	std::map<unsigned char, mvKey> keyStates;
	std::map<int, mvKey> specialStates;
};

#endif //MVINPUT