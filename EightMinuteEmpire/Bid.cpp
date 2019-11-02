#include "Bid.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <conio.h>

const int TWO_PLAYER_COIN_PURSE = 14;
const int THREE_PLAYER_COIN_PURSE = 11;
const int FOUR_PLAYER_COIN_PURSE = 9;
const int FIVE_PLAYER_COIN_PURSE = 8;


Bid::Bid(Player* p, int amountBid)
{
	player = p;
	this->amount = new int(amountBid);
}

int Bid::initiateBidding(Game* game)
{
	using namespace std;
	Player* winner;

	std::vector<Player*> players = game->players();
	int numPlayers = players.size();
	int bidLimit;

	switch (numPlayers)
	{
	case 2:
		bidLimit = TWO_PLAYER_COIN_PURSE;
		break;
	case 3:
		bidLimit = THREE_PLAYER_COIN_PURSE;
		break;
	case 4:
		bidLimit = FOUR_PLAYER_COIN_PURSE;
		break;
	case 5:
		bidLimit = FIVE_PLAYER_COIN_PURSE;
		break;
	default:
		break;
	}
	std::vector<Bid> bids;
	bids.reserve(numPlayers);

	std::cout << "\nBIDDING STARTED! \n";

	for (auto i = 0; i < numPlayers; i++)
	{
		cout << endl << players.at(i)->getName() << " enter the amount you wish to bid: " << endl;

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
				if ((stoi(in) < 0) || (stoi(in) > bidLimit))
				{
					cout << "\nPlease enter a value between 0 and " << bidLimit << ".\n";
					in.clear();
					continue;
				}
				break;
			}
		}
		cout << "\n" << players.at(i)->getName() << " has bid." << endl;
		Bid bid(players.at(i), stoi(in));
		bids.push_back(bid);
	}
	cout << "\nAll players have bid. The amounts were:\n";
	for (Bid i : bids)
	{
		cout << i;
	}
	winner = tallyBids(&bids);
	if (!winner)
		return 0;

	return decideOrder(game, winner);
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

	cout << endl << winner->getName() << " has won the bidding! They shall chose who plays first.\n\n";
	winner->PayCoin(*bids->at(0).amount);
	cout << *bids->at(0).amount << " coins has been deducted from " << winner->getName() << ".\n\n";


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

int Bid::decideOrder(Game* game, Player* winner)
{

	vector<Player*> players = game->players();

	cout << winner->getName() << " please enter who goes first:\n\n";

	int n = 1;
	for (Player* p : players)
	{
		cout << n << ". " << p->getName() << ((p->getName().compare(winner->getName()) == 0) ? " (You)" : "") << endl;
		n++;
	}
	cout << endl;
	int firstPlayer;
	cin >> firstPlayer;
	while ((firstPlayer > players.size() || firstPlayer < 1) || (!cin))
	{
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Please enter a number: " << endl;
			cin >> firstPlayer;
		}
		else if (firstPlayer > players.size() || firstPlayer < 1)
		{
			cout << "Please enter a number within: 1 and " << players.size() << endl;
			cin >> firstPlayer;
		}

	}
	firstPlayer -= 1;
	cout << players.at(firstPlayer)->getName() << " will go first!\n";

	return firstPlayer;

}

std::ostream& operator<<(std::ostream& s, const Bid& bid)
{
	return  s <<
		"Player: [" << bid.player->getName() << "] " <<
		"age: [" << bid.player->getAge() <<
		"] bid: " << *bid.amount << std::endl;

}