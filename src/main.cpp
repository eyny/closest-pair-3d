//Emre Yeniay
//BLG336E - Project 2

#include "ballmanager.h"
#include <iostream> //cout()

//The main function
int main(int argc, char** argv)
{
	//If there is no argument given
	if (argc == 1)
	{
		std::cout << "Error: No argument detected. Run the program with the name of the text file." << std::endl;
		std::cout << "Press ENTER key to exit..." << std::endl;
		getchar();
		return 1;
	}

	//Instance a ball manager
	BallManager myManager = BallManager(argv[1]);

	//Print outcomes to screen
	std::cout << "The distance is "  << myManager.ClosestPair() << std::endl;
	std::cout << "Number of total distance calculations is " << myManager.totalDistCalc << std::endl;
	std::cout << "Time spend to find minimum distance is " << myManager.execTime << " milliseconds" << std::endl;

	//Wait for enter input then terminate the program
	std::cout << "Press ENTER key to exit..." << std::endl;
	getchar();
	return 0;
}