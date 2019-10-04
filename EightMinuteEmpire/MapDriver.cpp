#include <iostream>
#include "Map.h"

using namespace std;
using namespace GraphWorld;

int main()
{
	string s1 = string("My Country");
	Country* c1 = new Country(7, false, &s1);
	Node* n1 = new Node(c1);
	cout << n1->displayNode() << "\n";
	delete n1;
	system("PAUSE");
	return 0;
}