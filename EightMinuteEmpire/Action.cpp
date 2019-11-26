#include "Action.h"
#include <sstream>

using namespace std;


Action::Action(int id) {
	this->id = new int(id);
	this->multiplicity = nullptr;
	this->isWaterMovementAllowed = nullptr;
	this->firstActionID = nullptr;
	this->secondActionID = nullptr;
	this->secondActionMultiplicity = nullptr;
	this->secondActionIsWaterMovementAllowed = nullptr;
}

Action::Action(int id, int multiplicity) {
	this->id = new int(id);
	this->multiplicity = new int(multiplicity);
	this->isWaterMovementAllowed = nullptr;
	this->firstActionID = nullptr;
	this->secondActionID = nullptr;
	this->secondActionMultiplicity = nullptr;
	this->secondActionIsWaterMovementAllowed = nullptr;
}

Action::Action(int id, int multiplicity, bool isWaterMovementAllowed) {
	this->id = new int(id);
	this->multiplicity = new int(multiplicity);
	this->isWaterMovementAllowed = new bool(isWaterMovementAllowed);
	this->firstActionID = nullptr;
	this->secondActionID = nullptr;
	this->secondActionMultiplicity = nullptr;
	this->secondActionIsWaterMovementAllowed = nullptr;
}

Action::Action(int id, int multiplicity = 1, bool isWaterMovementAllowed = false, int firstActionID = -1, int secondActionID = -1, int secondActionMultiplicity = 0, bool secondActionIsWaterMovementAllowed = false) {
	this->id = new int(id);
	this->multiplicity = new int(multiplicity);
	this->isWaterMovementAllowed = new bool(isWaterMovementAllowed);
	this->firstActionID = new int(firstActionID);
	this->secondActionID = new int(secondActionID);
	this->secondActionMultiplicity = new int(secondActionMultiplicity);
	this->secondActionIsWaterMovementAllowed = new bool(secondActionIsWaterMovementAllowed);
}

Action::Action(const Action& action)
{
	if (action.id != nullptr)
		id = new int(*(action.id));
	else
		id = nullptr;

	if (action.multiplicity != nullptr)
		multiplicity = new int(*(action.multiplicity));
	else
		multiplicity = nullptr;

	if (action.isWaterMovementAllowed != nullptr)
		isWaterMovementAllowed = new bool(*(action.isWaterMovementAllowed));
	else
		isWaterMovementAllowed = nullptr;

	if (action.firstActionID != nullptr)
		firstActionID  = new int(*(action.firstActionID));
	else
		firstActionID = nullptr;

	if (action.secondActionID != nullptr)
		secondActionID = new int(*(action.secondActionID));
	else
		secondActionID = nullptr;

	if (action.secondActionMultiplicity != nullptr)
		secondActionMultiplicity = new int(*(action.secondActionMultiplicity));
	else
		secondActionMultiplicity = nullptr;

	if (action.secondActionIsWaterMovementAllowed != nullptr)
		secondActionIsWaterMovementAllowed = new bool(*(action.secondActionIsWaterMovementAllowed));
	else
		secondActionIsWaterMovementAllowed = nullptr;

}

Action::~Action() {
	delete id;
	delete multiplicity;
	delete isWaterMovementAllowed;
	delete firstActionID;
	delete secondActionID;
	delete secondActionMultiplicity;
	delete secondActionIsWaterMovementAllowed;
}



std::string Action::actionString(bool printToConsole) {

	std::stringstream ss;
	int actionID = *id;

	if (actionID != 4 && actionID != 5) {

		switch (actionID)
		{
		case 0:
			ss << "PlaceNewArmies with " << *multiplicity << " armies. ";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 1:
			ss << "MoveArmies with " << *multiplicity << " armies and Water movement is " << *isWaterMovementAllowed;
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 2:
			ss << "BuildCity";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 3:
			ss << "DestroyArmy ";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 6:
			ss << "Ignore ";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		}
	}
	else {

		switch (*id)
		{
		case 4:
			ss << "AND ";
			break;
		case 5:
			ss << "OR ";
			break;
		}

		ss << " 1st Action = ";
		switch (*firstActionID)
		{
		case 0:
			ss << "PlaceNewArmies with " << *multiplicity << " armies. ";
			break;
		case 1:
			ss << "MoveArmies with " << *multiplicity << " armies and Water movement is " << *isWaterMovementAllowed;
			break;
		case 2:
			ss << "BuildCity ";
			break;
		case 3:
			ss << "DestroyArmy ";
			break;
		case 6:
			ss << "Ignore ";
			break;
		}

		ss << ", 2nd Action = ";
		switch (*secondActionID)
		{
		case 0:
			ss << "PlaceNewArmies with " << *secondActionMultiplicity << " armies. ";
			if(printToConsole)
			cout << ss.str();
			return ss.str();
		case 1:
			ss << "MoveArmies with " << *secondActionMultiplicity << " armies and Water movement is " << *secondActionIsWaterMovementAllowed;
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 2:
			ss << "BuildCity ";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 3:
			ss << "DestroyArmy ";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		case 6:
			ss << "Ignore ";
			if (printToConsole)
			cout << ss.str();
			return ss.str();
		}

	}

}

