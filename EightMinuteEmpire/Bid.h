#pragma once
#include "Player.h"
#include "Game.h"
#include <vector>


class Bid
{
public:
	Bid();
	Bid(Player* player, int amountBid);
	static int initiateBidding(Game* game);
	friend std::ostream& operator<<(std::ostream&, const Bid&);

private:
	Player* player;
	int* amount;
	static Player* tallyBids(std::vector<Bid>* bids);
	static Player* handleTie(std::vector<Bid>* highestBidders);
	static int decideOrder(Game* game, Player* winner);
	static Bid handleHumanBidding(Player* player, const int bidLimit);
	static Bid handleCPUBidding(Player* player, const int bidLimit);
};

