#pragma once
#include <iostream>  

using namespace std;

class Action
{
public:
	Action(int id);
	Action(int id, int multiplicity);
	Action(int id, int multiplicity, bool isWaterMovementAllowed);
	Action(int id, int multiplicity , bool isWaterMovementAllowed, int firstActionID, int secondActionID, int secondActionMultiplicity, bool secondActionIsWaterMovementAllowed);
	Action(const Action& action);
	~Action();
	int getID() { return *id; }
	int getMultiplicity() { return *multiplicity; }
	bool getisWaterMovementAllowed() { return *isWaterMovementAllowed; }
	int getFirstActionID() { return *firstActionID; }
	int getSecondActionID() { return *secondActionID; }
	int getSecondActionMultiplicity() { return *secondActionMultiplicity; }
	bool getSecondActionIsWaterMovementAllowed() { return *secondActionIsWaterMovementAllowed; }
	std::string actionString(bool printToConsole);

private:
	int* id;
	int* multiplicity;
	bool* isWaterMovementAllowed;
	int* firstActionID;
	int* secondActionID;
	int* secondActionMultiplicity;
	bool* secondActionIsWaterMovementAllowed;
};

