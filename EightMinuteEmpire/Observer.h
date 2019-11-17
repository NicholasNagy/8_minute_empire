#pragma once

// Observer Interface, to be concreted by concrete observers
class Observer {
public:
	~Observer();
	virtual void update() = 0;
protected:
	Observer();
};

