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
    
    //if (puck_position < 1 && puck_position > -1)
    //{
        difficulty = rand () % 5 + 1;

	    switch (difficulty) {
		    case 1:
            case 2:
                if (paddle2->pos.x < puck->pos.x)
				    return -1;
			    else if (paddle2->pos.x > puck->pos.x)
				    return 1;
			    else 
				    return 0;
			    break;

		    case 3:
                return 0;
                break;
		    case 4:
		    case 5:
			    if (paddle2->pos.x < puck->pos.x)
				    return 1;
			    else if (paddle2->pos.x > puck->pos.x)
				    return -1;
			    else 
				    return 0;
			    break;
		    default:
			    break;
        }
	//}
    //else
        return 0;

	//std::cout << "paddle_position:" << paddle_position << std::endl;
	//std::cout << "puck_position:" << puck_position << std::endl;
}
double alAI::updateZVelocity()
{
    if (puck->pos.z > 0)
    {
        if (puck->pos.z > paddle2->pos.z)
            return 1;
        else
            return -1;
    }
}
/*
double evaluatePuck (glm::vec3 paddle_position, glm::vec3 puck_position)
{
    int difficulty = rand () % 5 + 1;
    double ret = 0;
    if (puck_position.z < 0)
    {
        ret = 0;
    }
    else
    {
        if (puck_position->x < 1 && puck_position->x > -1)
        {
            

	        switch (difficulty) {
		        case 1:
			        if (paddle_position->x > puck_position->x)
				        ret = 1;
			        else if (paddle_position->x < puck_position->x)
				        ret = -1;
			        else 
				        ret = 0;
			        break;
		        case 2:
		        case 3:
                    ret = 0;
                    break;
		        case 4:
		        case 5:
			        if (paddle_position->x > puck_position->x)
				        ret = -1;
			        else if (paddle_position->x < puck_position->x)
				        ret = 1;
			        else 
				        ret = 0;
			        break;
		        default:
			        break;
            }
	    }
    }
    return ret;
}*/