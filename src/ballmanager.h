//Emre Yeniay
//BLG336E - Project 2

#ifndef BALLMANAGER_H
#define BALLMANAGER_H

#include "ball.h"
#include <string> //string
#include <vector> //vector

//Class for managing balls
class BallManager
{
	int ballCount; //Total number of balls
	Ball* ballArray; //Dynamically allocated ball array which holds all balls inside

	static bool CompareX(Ball, Ball); //Compare two balls according to X coordinate 
	static bool CompareY(Ball*, Ball*); //Compare two ball pointers according to Y coordinate 
	void FileOperations(std::string); //File operations
	double RecursiveCall(int, std::vector<Ball*>); //Recursive portion of the program
	double NaiveMethod(std::vector<Ball*>); //Naive method to find distance
	double SearchStrip(std::vector<Ball*>, double); //Search inside strip if a lesser distance is available

public:
	int totalDistCalc; //Total distance calculations
	int execTime; //Execution time of finding closest pair process

	BallManager(std::string); //Constructor
	~BallManager(); //Deconstructor
	double ClosestPair(); //Find the closest pair of balls
};

#endif