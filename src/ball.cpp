//Emre Yeniay
//BLG336E - Project 2

#include "ball.h"
#include <cmath> //sqrt()

//Calculate distance from this ball to target ball
double Ball::DistanceTo(Ball* targetBall)
{
	double squareDistX = (posX - targetBall->posX) * (posX - targetBall->posX);
	double squareDistY = (posY - targetBall->posY) * (posY - targetBall->posY);
	double squareDistZ = (posZ - targetBall->posZ) * (posZ - targetBall->posZ);
	return sqrt(squareDistX + squareDistY + squareDistZ);
}
