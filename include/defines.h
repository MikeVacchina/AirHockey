#ifndef DEFINES
#define DEFINES

//define key and mouse states to be used in input and framework classes
enum _keyState {KEY_UP, KEY_DOWN, KEY_INVALID};
enum _mouseState {MOUSE_UP, MOUSE_DOWN, MOUSE_INVALID};

//Define enu values for the different option available
const int RESTART		 = 1;
const int RESET			 = 2;
const int QUIT			 = 3;

//Define sub menu values for the different option available
const int TOGGLE_AI                     = 1;
const int TOGGLE_LIGHT_1                = 2;
const int TOGGLE_LIGHT_2                = 3;
const int INCREASE_PADDLE_SENSITIVITY   = 4;
const int DECREASE_PADDLE_SENSITIVITY   = 5;

//Define return values for the collision detection
const int INVALID	= -1;
const int GOAL1		= 1;
const int GOAL2		= 2;
const int NONE		= 3;

const int PUCK		= 0;
const int PADDLE1	= 1;
const int PADDLE2	= 2;
const int TABLE		= 3;


#endif //DEFINES