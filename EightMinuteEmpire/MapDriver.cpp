#include <iostream>
#include "Map.h"

using namespace std;
using namespace GraphWorld;

int main()
{
	string s1 = string("My Country");
	Country* c1 = new Country(7, true, &s1);
	string s2 = "MakramLand";
	Country* c2 = new Country(4, false, &s2);
	LinkedList* ll = new LinkedList(c1);
	ll->addToTail(c2);
	ll->displayLinkedList();
	delete ll;
	cout << "\n";
	system("PAUSE");
	return 0;
}