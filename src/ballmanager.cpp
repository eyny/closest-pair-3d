//Emre Yeniay
//BLG336E - Project 2

#include "ballmanager.h"
#include <iostream> //cout()
#include <fstream> //ifstream
#include <sstream> //istringstream
#include <algorithm> //sort(), min()
#include <chrono> //high_resolution_clock

//Constructor of the class
BallManager::BallManager(std::string fileName)
{
	ballCount = 0;
	totalDistCalc = 0;
	execTime = 0;
	FileOperations(fileName);
}

//Deconstructor of the class
BallManager::~BallManager()
{
	delete[] ballArray;
}

//Method for reading file and saving balls
void BallManager::FileOperations(std::string fileName)
{
	//String for a line and a word inside line
	std::string line, word;

	//Try to open file
	std::ifstream inputFile(fileName.c_str());
	if (!inputFile.is_open())
	{
		std::cout << "Error: The file cannot be found or opened." << std::endl;
		std::cout << "Press ENTER key to exit..." << std::endl;
		getchar();
		exit(1);
	}

	//Get first line of text as ball count
	getline(inputFile, line);
	ballCount = stoi(line);

	//Create new ball array
	ballArray = new Ball[ballCount];

	//Get other lines of text as ball positions
	for (int i = 0; i < ballCount; i++)
	{
		getline(inputFile, line);
		std::istringstream iss(line);

		getline(iss, word, ' ');
		ballArray[i].posX = stoi(word);

		getline(iss, word, ' ');
		ballArray[i].posY = stoi(word);

		getline(iss, word, '\n');
		ballArray[i].posZ = stoi(word);
	}

	//Close the file
	inputFile.close();
}



//Naive method to find minimum distance between points
double BallManager::NaiveMethod(std::vector<Ball*> balls)
{
	int size = balls.size();
	double minDist = std::numeric_limits<double>::max();

	//Take balls one by one
	for (int i = 0; i < size - 1; ++i)
	{
		//Check other remaining balls
		for (int j = i + 1; j < size; ++j)
		{
			double dist = balls[i]->DistanceTo(balls[j]);
			totalDistCalc++;
			if (dist < minDist)
				minDist = dist;
		}
	}
	return minDist;
}

//Find the distance beween the closest points of strip of given size
double BallManager::SearchStrip(std::vector<Ball*> strip, double stripDist)
{
	//Save the strip distance value as minimum distance known
	double minDist = stripDist;

	//If ball count in strip is less than 2, return strip distance
	int stripSize = strip.size();
	if (stripSize < 2)
		return minDist;

	//If ball count in strip is 2 or 3, find distance with naive method
	if (stripSize < 4)
	{
		double newDist = NaiveMethod(strip);
		if (newDist < minDist)
			minDist = newDist;
		return minDist;
	}

	//If ball count is more than 3, start processing of creating columns
	//Find the smallest z value among balls
	int smallestZ = strip[0]->posZ;
	for (int i = 1; i < stripSize; i++)
		if (strip[i]->posZ < smallestZ)
			smallestZ = strip[i]->posZ;

	//Find the largest z value among balls
	int largestZ = strip[0]->posZ;
	for (int i = 1; i < stripSize; i++)
		if (strip[i]->posZ > largestZ)
			largestZ = strip[i]->posZ;

	//Find range between z values and column count
	int rangeZ = largestZ - smallestZ;
	int columnCount = (int)(rangeZ / stripDist) + 1;

	//Create an array of ball pointer vectors to put pointers of balls into proper columns
	std::vector<Ball*>* columns = new std::vector<Ball*>[columnCount];

	//Put all balls into proper columns (Phase 1)
	//Start picking balls one by one with the smallest Y first
	for (int i = 0; i < stripSize; i++)
	{
		//Find middle(main) column of ball, put ball into it, save index
		strip[i]->columnNumber[1] = (int)((double)(strip[i]->posZ - smallestZ) / stripDist);
		columns[strip[i]->columnNumber[1]].push_back(strip[i]);
		strip[i]->columnIndex[1] = columns[strip[i]->columnNumber[1]].size() - 1;

		//Find and save left and right columns of ball
		strip[i]->columnNumber[0] = strip[i]->columnNumber[1] - 1;
		strip[i]->columnNumber[2] = strip[i]->columnNumber[1] + 1;

		//There is no column at left of leftmost column, and right of rightmost column
		//Save indices in left and right columns
		if (strip[i]->columnNumber[1] != 0) //If not leftmost column
			strip[i]->columnIndex[0] = columns[strip[i]->columnNumber[0]].size() - 1;

		if (strip[i]->columnNumber[1] != columnCount - 1) //If not rightmost column
			strip[i]->columnIndex[2] = columns[strip[i]->columnNumber[2]].size() - 1;
	}

	//Find if such lesser distance exists between any pair of balls inside strip (Phase 2)
	//Start picking balls one by one with the smallest Y first
	for (int i = 0; i < stripSize; i++)
	{
		//For left, middle and right columns of the selected ball
		for (int j = 0; j < 3; j++)
		{
			//Skip invalid out of bounds columns
			if ((strip[i]->columnNumber[j] == -1) || (strip[i]->columnNumber[j] == columnCount))
				continue;

			//Look through increasing Y value = O(1)
			for (int k = strip[i]->columnIndex[j] + 1; k < (int)columns[strip[i]->columnNumber[j]].size(); k++)
			{
				//If Y distance is already more than strip distance, no need to check remaning balls in column
				if (columns[strip[i]->columnNumber[j]][k]->posY > strip[i]->posY + stripDist)
					break;

				//Check distance and if it is smaller than current minimum distance, save it as new minimum distance
				double newDist = strip[i]->DistanceTo(columns[strip[i]->columnNumber[j]][k]);
				totalDistCalc++;
				if (newDist < minDist)
					minDist = newDist;
			}
		}
	}

	//Delete dynamically allocated columns array
	delete[] columns;

	//Return minimum distance found in strip or initially given strip distance
	return minDist;
}

//Divide array and conquer value recursively
double BallManager::RecursiveCall(int sortedXPos, std::vector<Ball*> sortedY)
{
	//If size is small enough, find distance with naive method
	int size = sortedY.size();
	if (size < 4)
		return NaiveMethod(sortedY);

	//Get middle ball for X axis
	int midIndex = sortedXPos + size / 2;
	Ball* midBall = &ballArray[midIndex];

	//Initiliaze left and right subarrays
	std::vector<Ball*> leftVector;
	std::vector<Ball*> rightVector;

	//In case of there are other balls which share same X position as middle point
	bool putToRightArr = true;

	//Distribute balls into left and right vectors
	for (int i = 0; i < size; i++)
	{
		if (sortedY[i]->posX < midBall->posX)
			leftVector.push_back(sortedY[i]);
		else if (sortedY[i]->posX > midBall->posX)
			rightVector.push_back(sortedY[i]);
		else //if (sortedY[i].posX == midBall.posX)
		{
			if (putToRightArr)
				rightVector.push_back(sortedY[i]);
			else
				leftVector.push_back(sortedY[i]);
			putToRightArr = !putToRightArr;
		}
	}

	//Pass subarrays to same function recursively
	double minLeftDist = RecursiveCall(sortedXPos, leftVector);
	double minRightDist = RecursiveCall(sortedXPos + leftVector.size(), rightVector);

	//Initiliaze strip vector
	std::vector<Ball*> strip;
	double stripDist = std::min(minLeftDist, minRightDist);

	//Put balls which are close enough to middle into strip
	for (int i = 0; i < size; i++)
	{
		int distToMid = sortedY[i]->posX - midBall->posX;
		if (distToMid < 0)
			distToMid *= -1;
		if (distToMid < stripDist)
			strip.push_back(sortedY[i]);
	}

	//Search in strip and return the minimum distance
	return SearchStrip(strip, stripDist);
}

//Compare two balls according to X coordinate for std::sort
bool BallManager::CompareX(Ball ball1, Ball ball2)
{
	return (ball1.posX < ball2.posX);
}

//Compare two ball pointers according to Y coordinate for std::sort
bool BallManager::CompareY(Ball* ball1, Ball* ball2)
{
	return (ball1->posY < ball2->posY);
}

//First call from main to find closest pair
double BallManager::ClosestPair()
{
	//Record starting time
	auto started = std::chrono::high_resolution_clock::now();

	//Sort original ballArray by increasing X order = O(nlogn)
	std::sort(ballArray, ballArray + ballCount, &BallManager::CompareX);

	//Initiliaze sortedY vector
	std::vector<Ball*> sortedY;
	for (int i = 0; i < ballCount; i++)
		sortedY.push_back(&ballArray[i]);

	//Sort sortedY by increasing Y order = O(nlogn)
	std::sort(sortedY.begin(), sortedY.end(), &BallManager::CompareY);

	//Start recursive part of the program and get result
	double finalMinDist = RecursiveCall(0, sortedY);

	//Record finishing time and save time passed
	auto finished = std::chrono::high_resolution_clock::now();
	execTime = (int)std::chrono::duration<double, std::milli>(finished - started).count();

	//Return the final minimum distance
	return finalMinDist;
}