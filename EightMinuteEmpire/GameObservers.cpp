#include "GameObservers.h"
#include <iostream>


PhaseObserver::PhaseObserver()
{
	observableGameplayState = nullptr;
}

PhaseObserver::PhaseObserver(Observable* gameplayState)
{
	observableGameplayState = gameplayState;
	observableGameplayState->attach(this); //add the PhaseObserver to the list of observers of the game
}

PhaseObserver::~PhaseObserver()
{
	observableGameplayState->detach(this); //detach the PhaseObserver from the list of observers of the game when desturcted
}

void PhaseObserver::setObservableGameplayState(Observable* gameplayState)
{
	std::cout << "in set ObservableGameplayState\n";
	observableGameplayState = gameplayState;
	observableGameplayState->attach(this); //add the PhaseObserver to the list of observers of the game
}

void PhaseObserver::update()
{
	currentStatus = observableGameplayState->getStatus();
}


