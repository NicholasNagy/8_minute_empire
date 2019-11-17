#pragma once
#include <list>
#include "Observer.h"
#include <string>

using namespace std;


// Observer Interface, to be concreted by concrete observables (subjects)
class Observable {
private:
	// a vector of observers (subscribers) that
	// want to be notified when a change happens in the observable
	list<Observer*>* observersList;
	string status;
	
public:
	Observable();
	~Observable();
	virtual void attach(Observer* o); // attach observer to the list of observers (subscribers)
	virtual void detach(Observer* o); // detach observer to the list of observers (subscribers)
	virtual void notify(); // notify all observers in the list of observers (subscribers) that a change has happend
	virtual string getStatus() { return status; }
	virtual void startNewStatus(string newStatus) { status = newStatus; notify(); }
	virtual void updateStatus(string addStatus) { status = status + "\n" + addStatus; notify(); }
};

