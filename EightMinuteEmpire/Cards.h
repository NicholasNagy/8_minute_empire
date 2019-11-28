#pragma once

#include <vector>  
#include <iostream>  
#include <sstream>
#include "Action.h"

using std::cout;

class Card
{
	int* good;
	Action* action;

	protected:
		Card();
		Card(const int myGood, const Action& myAction);

	public:
		Card(const Card& c); // Copy constructor
		Card* Create(string type);
		Card & operator=(const Card& c); // Overload assignment operator
		friend ostream& operator<<(ostream& os, const Card& card);
		~Card();
		void printCard()
		{ 
			cout << "Card: Goods = " << *good << ", ";
			action->actionString(true);
		}
		Action* getAction();
		int getGood();
};

class Deck
{
	static const int NumOfCardsInDeck = 42;
	vector<Card* >* cardsDeck = new vector<Card* >(42);

	public:
		Deck();
		~Deck();
		void printDeck();
		Card* draw(); // Draw the top card from the deck.
		void shuffleDeck(); // Shuffle the cards in the deck.
};



class Hand
{
	vector<Card* >* cardsHand = new vector<Card* >(6);

	public:
		Hand();
		Hand(Deck* newDeck);
		~Hand();
		void printHand();
		int getCardCostAtPosition(int postion);
		Card* getCardAtPosition(int postion, Deck* deck, bool MoveCards = true);
		void moveCards(int postionOfTheRemovedCard, Deck* deck);
		vector<Card* > cardsInHand();
};