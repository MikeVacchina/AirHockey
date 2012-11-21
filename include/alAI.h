#ifndef ALAI
#define ALAI

#include <map>

#include <Windows.h>
#include <gl/glew.h> // glew must be included before the main gl libs
#include <gl/glut.h> // doing otherwise causes compiler shouting

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <time.h>

#include "mvPuck.h"
#include "mvPaddle.h"
#include "mvSimpleStructs.h"

//this class handles user input throught the mouse and keyboard
class alAI
{
public:
	alAI();
	~alAI();
	
	//return x Position
	double updateXVelocity();
	double updateZVelocity();
    //double evaluatePuck (glm::vec3 , glm::vec3 );
	void setPuck(mvPuck *p);
	void setPaddle2(mvPaddle *p);
	//double updateYVelocity();

private:
	int difficulty;

	mvPuck *puck;

	mvPaddle *paddle2;
};

#endif //ALAI