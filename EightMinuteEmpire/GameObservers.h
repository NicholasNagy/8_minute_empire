#pragma once
#include "Observer.h"
#include "Observable.h"
#include <string>





// PhaseObserver is a concrete Observer that will observe be updated when game changes
class PhaseObserver : public Observer
{
private:
	// we need the game as a subject so when it notifies the PhaseObserver of a change
	// the phase observer will call gmae.getState() and display the changes.
	static Observable* observableGameplayState;
	std::string currentStatus; // Stores the current game status

public:
	PhaseObserver();
	PhaseObserver(Observable* gameplayState); // the passed game will set the subject data member
	~PhaseObserver();
	void setObservableGameplayState(Observable* gameplayState);
	void update();
	std::string getStatus() { return currentStatus; }; // Display the current changes in the UI
};




// StatsObserver is a concrete Observer that will observe be updated when player stats change
class StatsObserver : public Observer
{
private:
	// we need the game as a subject so when it notifies the StatsObserver of a change
	// the StatsObserver will call gmae.getState() and display the changes.
	static Observable* observableGameplayState;
	std::string statistics; // Stores all the game statistics

public:
	StatsObserver();
	StatsObserver(Observable* gameplayState); // the passed game will set the subject data member
	~StatsObserver();
	void setObservableGameplayState(Observable* gameplayState);
	void update();
	std::string getStatistics() { return statistics; }; // Display the current changes in the UI

};



