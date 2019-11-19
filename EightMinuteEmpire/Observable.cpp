#include "Observable.h"

using namespace std;

// Code inspired from COMP345 design patterns slides with Dr. Nora. 
Observable::Observable()
{
	observersList = new list<Observer*>;
}

Observable::~Observable()
{
	delete observersList;
}

void Observable::attach(Observer* observer)
{
	observersList->push_back(observer);
}

void Observable::detach(Observer* observer)
{
	observersList->remove(observer);
}

// call the update method of all the registerd observers in the observersList 
void Observable::notify()
{
	list<Observer*>::iterator i = observersList->begin();
	for (; i != observersList->end(); ++i)
	{
		(*i)->update();
	}
	
}


void Observable::startNewStatus(string newStatus)
{
	status.clear();
	status.push_back(newStatus);
	notify();
}

string Observable::getStatus()
{
	stringstream ss;

	for (string s : status)
		ss << s << "\n";

	return ss.str();
}

void Observable::updateStatus(string addStatus)
{
	status.push_back(addStatus);

	if (status.size() > 4)
		status.pop_front();

	notify();
}