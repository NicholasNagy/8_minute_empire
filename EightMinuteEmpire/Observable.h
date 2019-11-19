#pragma once
#include <list>
#include "Observer.h"
#include <string>
#include <deque>
#include <sstream>
#include <iostream>
#include "player.h"
#include <vector>

using namespace std;


// Observer Interface, to be concreted by concrete observables (subjects)
class Observable {
private:
	// a vector of observers (subscribers) that
	// want to be notified when a change happens in the observable
	list<Observer*>* observersList;
	std::deque<string> status;  //Keeps the last 4 status messages. Once it get another message over 4, it pops the first message
	string statistics;

public:
	Observable();
	~Observable();
	virtual void attach(Observer* o); // attach observer to the list of observers (subscribers)
	virtual void detach(Observer* o); // detach observer to the list of observers (subscribers)
	virtual void notify(); // notify all observers in the list of observers (subscribers) that a change has happend
	virtual string getStatus();
	virtual void startNewStatus(string newStatus);
	virtual void updateStatus(string addStatus);
	virtual string getStatistics() { return statistics; };
	virtual void setStatitics(string newStats) { statistics = newStats; };
};

