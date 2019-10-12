#include "Bid.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <conio.h>


Bid::Bid(Player* p, int amountBid)
{
	player = p;
	this->amount = new int(amountBid);
}

Player* Bid::initiateBidding(Player players[], const int numPlayers)
{
	using namespace std;

	std::vector<Bid> bids;
	bids.reserve(numPlayers);

	std::cout << "\nBIDDING STARTED! \n";

	for (auto i = 0; i < numPlayers; i++)
	{
		cout << endl << players[i].getName() << " enter the amount you wish to bid: " << endl;

		int x;
		string in;

		while ((x = _getch()))
		{
			if (isdigit(x))
			{
				cout << '*';
				x = x - '0';
				in += to_string(x);
				// cout << x;
				continue;
			}
			if (x == '\b' && in.size() > 0)
			{
				in.pop_back();
				cout << '\b';
				cout << " ";
				cout << '\b';
				continue;
			}
			if (x == '\r' && in.size() > 0)
			{
				if ((stoi(in) < 0) || (stoi(in) > 20))
				{
					cout << "\nPlease enter a value between 0 and 20:\n";
					in.clear();
					continue;
				}
				break;
			}
		}
		cout << "\nConsole: " << players[i].getName() << " has bid." << endl;
		Bid bid(&players[i], stoi(in));
		bids.push_back(bid);
	}
	cout << "\nAll players have bid. The amounts were:\n";
	for (Bid i : bids)
	{
		cout << i;
	}

	return tallyBids(&bids);
}

Player* Bid::tallyBids(std::vector<Bid>* bids)
{
	Player* winner;

	sort(bids->begin(), bids->end(), [](const auto& v1, const auto& v2)
	{
		return *v1.amount > * v2.amount;
	});

	std::vector<Bid> highestBidders;
	highestBidders.push_back(bids->at(0));

	//Checking for ties
	for (int i = 1; i < bids->size(); i++)
	{
		if (*bids->at(0).amount == *bids->at(i).amount)
			highestBidders.push_back(bids->at(i));
	}

	if (highestBidders.size() >= 2)
		winner = handleTie(&highestBidders);
	else
		winner = bids->at(0).player;

	cout << endl << winner->getName() << " has won the bidding! They shall chose who plays first.\n";
	/*	winner->PayCoin(*bids->at(0).amount);
		cout << *bids->at(0).amount << " coins has been deducted from " << winner->getName() << ".\n";
		*/

	return winner;
}

Player* Bid::handleTie(std::vector<Bid>* highestBidders)
{
	sort(highestBidders->begin(), highestBidders->end(), [](const auto& v1, const auto& v2)
	{
		return v1.player->getAge() < v2.player->getAge();
	});

	return highestBidders->at(0).player;
}

std::ostream& operator<<(std::ostream& s, const Bid& bid)
{
	return  s <<
		"Player: [" << bid.player->getName() << "] " <<
		"age: [" << bid.player->getAge() <<
		"] bid: " << *bid.amount << std::endl;

}