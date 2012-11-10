#ifndef MVMATH
#define MVMATH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

//returns the smallest distance between a point (pt) and line segment (segA, segB)
double distanceLineSegPt(glm::vec2 segA, glm::vec2 segB, glm::vec2 pt);

#endif //MVMATH