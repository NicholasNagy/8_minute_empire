#include <iostream>
#include "Map.h"

using namespace std;
using namespace GraphWorld;

int main()
{
	string s1 = string("My Country");
	Country* c1 = new Country(1, true, &s1);
	string s2 = "MakramLand";
	Country* c2 = new Country(2, false, &s2);
	cout << c2->getContinent() + "\n\n";
	Country* c3 = new Country(3, false, &s2);
	Country* c4 = new Country(4, false, &s2);
	LinkedList* ll = new LinkedList(c1);
	ll->add(0, c2);
	ll->add(2, c3);
	ll->add(2, c4);
	ll->displayLinkedList();
	cout << "\nRemoving Two Items\n";
	ll->remove(0);
	ll->remove(3);
	ll->displayLinkedList();
	delete ll;
	cout << "\n";
	system("PAUSE");
	return 0;
}