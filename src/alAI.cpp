#include "alAI.h"


alAI::alAI()
{
    srand ( time(NULL) );
    difficulty = 5;

	puck = NULL;

	paddle2 = NULL;
}


alAI::~alAI()
{
}

void alAI::setPuck(mvPuck *p)
{
	puck = p;
}

void alAI::setPaddle2(mvPaddle *p)
{
	paddle2 = p;
}
double alAI::updateXVelocity()
{
	//get length of time key was/is down
    difficulty = rand () % 5 + 1;
    if (puck->pos.z > 0)
    {
        switch (difficulty) 
        {
		    case 1:
            case 2:
            case 3:
            case 4:
                return 0;
                break;
		    case 5:
			    if (paddle2->pos.x < puck->pos.x)
				    return 0.1;
			    else if (paddle2->pos.x > puck->pos.x)
				    return -0.1;
			    else 
				    return 0;
			    break;
		    default:
			    break;
        }
    }
    else
    {
        if (paddle2->pos.x < 0)
        {
	        switch (difficulty) 
            {
		        case 1:
                case 2:
		        case 3:
		        case 4:
                    return 0;
                    break;
		        case 5:
                    return 0.2;
		        default:
			        break;
            }
        }
        else if (paddle2->pos.x > 0)
        {
            switch (difficulty) 
            {
		        case 1:
                case 2:
		        case 3:
                case 4:
                    return 0;
                    break;
		        case 5:
                    return -0.2;
		        default:
			        break;
            }
        }
    }
    return 0;
}
double alAI::updateZVelocity()
{
    if (puck->pos.z == -3.0)
    {
       return 1;
    }

    /*
    difficulty = rand () % 5 + 1;
    if (puck->pos.z > 0)
    {
        switch (difficulty) 
        {
		    case 1:
            case 2:
		    case 3:
            case 4:
                return 0;
                break;

		    case 5:
                return 1;
		    default:
			    break;
        }
    }
    else
    {
        if (puck->pos.z < 3)
        {
            switch (difficulty) 
            {
		        case 1:
                case 2:
		        case 3:
                case 4:
                    return 0;
                    break;

		        case 5:
                    return -1;
		        default:
			        break;
            }
        }
    }
    */
    return 0;
}
