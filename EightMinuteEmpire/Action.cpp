#include "Action.h"
#include <iostream>

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



void Action::printAction() {

	int actionID = *id;

	if (actionID != 4 && actionID != 5) {

		switch (actionID)
		{
		case 0:
			cout << "Action = PlaceNewArmies with " << *multiplicity << " armies. ";
			return;
		case 1:
			cout << "Action = MoveArmies with " << *multiplicity << " armies and Water movement is " << *isWaterMovementAllowed;
			return;
		case 2:
			cout << "Action = BuildCity";
			return;
		case 3:
			cout << "Action = DestroyArmy ";
			return;
		case 6:
			cout << "Action = Ignore ";
			return;
		}
	}
	else {

		switch (*id)
		{
		case 4:
			cout << "Action = AND ";
			break;
		case 5:
			cout << "Action = OR ";
			break;
		}

		cout << " First Action = ";
		switch (*firstActionID)
		{
		case 0:
			cout << "PlaceNewArmies with " << *multiplicity << " armies. ";
			break;
		case 1:
			cout << "MoveArmies with " << *multiplicity << " armies and Water movement is " << *isWaterMovementAllowed;
			break;
		case 2:
			cout << "BuildCity ";
			break;
		case 3:
			cout << "DestroyArmy ";
			break;
		case 6:
			cout << "Ignore ";
			break;
		}

		cout << ", Second Action = ";
		switch (*secondActionID)
		{
		case 0:
			cout << "PlaceNewArmies with " << *secondActionMultiplicity << " armies. ";
			break;
		case 1:
			cout << "MoveArmies with " << *secondActionMultiplicity << " armies and Water movement is " << *secondActionIsWaterMovementAllowed;
			break;
		case 2:
			cout << "BuildCity ";
			break;
		case 3:
			cout << "DestroyArmy ";
			break;
		case 6:
			cout << "Ignore ";
			break;
		}

	}
}

