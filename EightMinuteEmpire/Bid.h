#pragma once
#include "Player.h"
#include "Game.h"
#include <vector>

class Bid
{
public:
	Bid(Player* player, int amountBid);
	static Player* initiateBidding(Game* game);
	friend std::ostream& operator<<(std::ostream&, const Bid&);

private:
	Player* player;
	int* amount;
	static Player* tallyBids(std::vector<Bid>* bids);
	static Player* handleTie(std::vector<Bid>* highestBidders);

};

