#pragma once
#include <vector>  
using namespace std;


class Card
{
public:
	Card();
	Card(int myGood, int myAction);
	~Card();
	void setParameters(int good, int action);
	void printCard();
	int getGood();
	int getAction();

private:
	int* good;
	int* action;
};


class Deck
{
public:
	Deck();
	~Deck();
	void printDeck();
	Card draw(); // Draw the top card from the deck.

private:
	const int* NumOfCardsInDeck = new int(42);
	vector<Card>* cardsDeck = new vector<Card>(*NumOfCardsInDeck);
};

// HandSlot class has a card with a corressponding value. To be used by the Hand class.
class HandSlot
{
public:
	HandSlot();
	HandSlot(int newSlotValue, Card newSlotCard);
	~HandSlot();
	void printHandSlot();
	void setParameters(int newSlotValue, Card newSlotCard);

private:
	int* slotValue;
	Card* slotCard;
};



class Hand
{
public:
	Hand();
	Hand(Deck& newDeck);
	~Hand();
	void printHand();

private:
	const int* NumOfCardsInHand = new int(6);
	vector<HandSlot>* cardsHand = new vector<HandSlot>(*NumOfCardsInHand);
};