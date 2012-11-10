#ifndef DEFINES
#define DEFINES

//define key and mouse states to be used in input and framework classes
enum _keyState {KEY_UP, KEY_DOWN, KEY_INVALID};
enum _mouseState {MOUSE_UP, MOUSE_DOWN, MOUSE_INVALID};

//Define enu values for the different option available
const int PLAYMAZEONE	= 1;
const int PLAYMAZETWO	= 2;
const int RESTART		= 3;
const int QUIT			= 4;

//Define sub menu values for the different option available
const int INCMOUSE		= 1;
const int DECMOUSE		= 2;
const int INCKEY		= 3;
const int DECKEY		= 4;
const int INCGRAVITY	= 5;
const int DECGRAVITY	= 6;
const int INCBOUNCE		= 7;
const int DECBOUNCE		= 8;
const int TOGLIGHTONE	= 10;
const int TOGLIGHTTWO	= 11;
const int RESET			= 9;

//Define return values for the collision detection
const int INVALID	= -1;
const int GOAL		= 1;
const int HOLE		= 2;
const int NONE		= 3;


#endif //DEFINES