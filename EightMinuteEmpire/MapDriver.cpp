#include <iostream>
#include "Map.h"

using namespace std;
using namespace GraphWorld;

int main()
{
	string s1 = string("My Country");
	Country* c1 = new Country(7, false, &s1);
	cout << c1->displayCountry() << "\n";
	delete c1;
	system("PAUSE");
	return 0;
}