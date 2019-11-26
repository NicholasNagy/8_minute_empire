#pragma once
#include <vector>  
#include <iostream>  
#include <sstream>
#include "Action.h"
using namespace std;



class Card
{
public:
	Card();
	Card(const int myGood, const Action& myAction);
	Card(const Card& c); // Copy constructor
	Card & operator=(const Card& c); // Overload assignment operator
	friend ostream& operator<<(ostream& os, const Card& card);
	~Card();
	void printCard() { 
		cout << "Card: Goods = " << *good << ", ";
		action->actionString(true);
	}
	Action* getAction();
	int getGood();
private:
	int* good;
	Action* action;
};



class Deck
{
public:
	Deck();
	~Deck();
	void printDeck();
	Card* draw(); // Draw the top card from the deck.
	void shuffleDeck(); // Shuffle the cards in the deck.


private:
	static const int NumOfCardsInDeck = 42;
	vector<Card * >* cardsDeck = new vector<Card * >(42);
};



class Hand
{
public:
	Hand();
	Hand(Deck* newDeck);
	~Hand();
	void printHand();
	int getCardCostAtPosition(int postion);
	Card* getCardAtPosition(int postion, Deck* deck);
	void moveCards(int postionOfTheRemovedCard, Deck* deck);
	vector<Card* > cardsInHand();
	

private:
	vector<Card* >* cardsHand = new vector<Card* >(6);
};