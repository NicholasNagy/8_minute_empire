#pragma once
#include <iostream>  

using namespace std;

class Action
{
public:

	Action(int id, int multiplicity, bool isWaterMovementAllowed, int firstActionID, int secondActionID, int secondActionMultiplicity, bool secondActionIsWaterMovementAllowed);
	int getID() { return id; }
	int getMultiplicity() { return multiplicity; }
	bool isWaterMovementAllowed() { return mIsWaterMovementAllowed; }
	int getFirstActionID() { return firstActionID; }
	int getSecondActionID() { return secondActionID; }
	int getSecondActionMultiplicity() { return secondActionMultiplicity; }
	bool getSecondActionIsWaterMovementAllowed() { return secondActionIsWaterMovementAllowed; }

private:
	int id;
	int multiplicity;
	bool mIsWaterMovementAllowed;
	int firstActionID;
	int secondActionID;
	int secondActionMultiplicity;
	bool secondActionIsWaterMovementAllowed;
};

