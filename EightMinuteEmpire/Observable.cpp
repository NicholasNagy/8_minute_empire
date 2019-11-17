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
		(*i)->update();
}