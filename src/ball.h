//Emre Yeniay
//BLG336E - Project 2

#ifndef BALL_H
#define BALL_H

//Class for balls
class Ball
{
public:
        double posX, posY, posZ;
	int columnNumber[3]; //ID number of the column which ball belongs to (left, middle, right)
	int columnIndex[3]; //Position inside the column (left, middle, right)
	double DistanceTo(Ball*); //Find distance to another ball
};

#endif
