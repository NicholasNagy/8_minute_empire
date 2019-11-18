#pragma once
#include <list>
#include "Observer.h"
#include <string>
#include <deque>
#include <sstream>
#include <iostream>

using namespace std;


// Observer Interface, to be concreted by concrete observables (subjects)
class Observable {
private:
	// a vector of observers (subscribers) that
	// want to be notified when a change happens in the observable
	list<Observer*>* observersList;
	std::deque<string> status;  //Keeps the last 3 status messages. Once it get another message over 3, it pops the first message
	
public:
	Observable();
	~Observable();
	virtual void attach(Observer* o); // attach observer to the list of observers (subscribers)
	virtual void detach(Observer* o); // detach observer to the list of observers (subscribers)
	virtual void notify(); // notify all observers in the list of observers (subscribers) that a change has happend
	
	virtual void startNewStatus(string newStatus)
	{ 
		status.clear();
		status.push_back(newStatus); 
		notify(); 
	}

	virtual string getStatus()
	{ 
		stringstream ss;

		for (string s : status)
			ss << s << "\n";

		return ss.str();
	}

	virtual void updateStatus(string addStatus)
	{ 
		status.push_back(addStatus);

		if (status.size() > 4)
			status.pop_front();

		notify(); 
	}
};

