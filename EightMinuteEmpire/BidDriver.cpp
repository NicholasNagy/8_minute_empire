#include "Player.h"
#include "Bid.h"
#include <iostream>

int main() {

	using namespace std;

	string p1 = "player1";
	string p2 = "player2";
	string p3 = "player3";
	string p4 = "player4";

	const int numPlayers = 5;
	Player player1;
	Player player2(&p1, 18);
	Player player3(&p2, 22);
	Player player4(&p3, 12);
	Player player5(&p4, 68);

	Player players[numPlayers] = { player1, player2, player3, player4, player5 };
	std::cout << "Bidders are:\n";
	for (auto i : players)
		cout << i;

	Bid::initiateBidding(players, numPlayers);




	system("PAUSE");
	return 0;
}