/*#include <iostream>
#include "Map.h"

using namespace std;
using namespace GraphWorld;

int main()
{
	string s1 = string("My Country");
	Country* c1 = new Country(0, true,true, &s1);
	string s2 = "MakramLand";
	Country* c2 = new Country(1, false,false, &s2);
	Country* c3 = new Country(2, false,true, &s2);
	Country* c4 = new Country(3, false,false, &s2);
	string s3 = "Avery's FUCKING MAP";
	Map* graphy = new Map(&s3, 4, 1);
	graphy->addNode(c1);
	graphy->addNode(c2);
	graphy->addNode(c3);
	graphy->addNode(c4);
	graphy->addEdge(c1, c2, true);
	graphy->addEdge(c1, c3,true);
	graphy->addEdge(c1, c4,true);
	graphy->addEdge(c4, c2,true);
	graphy->printMap();
	cout << "\nLinkedList Part:\n";
	LinkedList* ll = new LinkedList(c1);
	ll->add(0, c2,true);
	ll->add(2, c3,true);
	ll->add(2, c4,false);
	Country* ct = ll->get(1);
	cout << ct->displayCountry();
	cout << "\nRemoving Two Items\n";
	ll->remove(0);
	ll->remove(3);
	ll->displayLinkedList();
	delete ll;
	cout << "\n";
	system("PAUSE");
	return 0;
}
*/