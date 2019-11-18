#include "GameObservers.h"
#include <iostream>

Observable* PhaseObserver::observableGameplayState = nullptr;

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
	observableGameplayState = gameplayState;
	observableGameplayState->attach(this); //add the PhaseObserver to the list of observers of the game
}

void PhaseObserver::update()
{
	if (observableGameplayState)
	currentStatus = PhaseObserver::observableGameplayState->getStatus();
}


