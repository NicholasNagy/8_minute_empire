//#include <iostream>
//#include <unordered_set>
//#include <map>
//#include <set>
//#include <vector>
//
//class Holding
//{
//public:
//	Holding() {
//		numArmies = 0;
//		numCities = 10;
//	}
//	~Holding()
//	{
//		delete this;
//	}
//
//public:
//	int numArmies;
//	int numCities;
//
//};
//
//struct Cmp
//{
//	bool operator ()(const std::pair<int, Holding*>& a, const std::pair<int, Holding*>& b) const
//	{
//		return a.second > b.second;
//	}
//};
//
//int main()
//{
//
//	using namespace std;
//	std::unordered_set<Holding*> occuppiedCountries;
//
//
//
//	Holding* h = new Holding();
//	Holding* g = new Holding();
//	occuppiedCountries.emplace(h);
//
//
//
//	Holding* a = new Holding();
//	Holding* b = new	Holding();
//
//
//std::set<std::pair<int, Holding*>, Cmp> mapper;
//std::vector<Holding*> vec;
//
//	vec.push_back(a);
//	vec.push_back(b);
//	
//	mapper.emplace(0,vec.at(0));
//	mapper.emplace(0, vec.at(1));
//
//
//	//auto it = std::find_if(mapper.begin(), mapper.end(),
//	//	[]( const std::pair<int, Holding*>& element)
//	//{ 
//	//	if (element.first == 1)
//	//	{
//
//	//		//element.first = 10;
//	//		cout << element.first << endl;
//	//	}
//	//	return element.first == 1;
//	//});
//
//
//	
//	auto it = mapper.begin();
//	for (int i = 0; i < mapper.size(); ++i)
//	{
//		std::cout << it->first << "--" << it->second << std::endl;
//		++it;
//
//	}
//
//	cout << mapper.size() << endl;
//
//	system("PAUSE");
//	return 0;
//}