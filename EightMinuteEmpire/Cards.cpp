#include "Cards.h"
#include <iostream>
#include <vector>  
using namespace std;


Card::Card() {
	this->good = nullptr;
	this->action = nullptr;
}

Card::Card(int myGood, int myAction) {
	this->good = new int(myGood);
	this->action = new int(myAction);
}

Card::~Card() {
	//delete good;
	//delete action;
}

// Sets the parameters of the Card.
void Card::setParameters(int myGood, int myAction)
{
	this->good = new int(myGood);
	this->action = new int(myAction);
}

// Print the Card details.
void Card::printCard() {
	cout << "Card details: Good = " << *good << ", action = " << *action << endl;
}

// Getters.
int Card::getGood()
{
	return *good;
}

int Card::getAction()
{
	return *action;
}


// Create teh deck object by filling the vector with 42 cards.
Deck::Deck()
{
	for (int i = 0; i < *NumOfCardsInDeck; i++) {
		this->cardsDeck->at(i).setParameters(i, i);
	}

	cout << "Deck of cards is now instantiated and has: " << cardsDeck->size() << " cards\n";
}

Deck::~Deck()
{
}

void Deck::printDeck()
{
	cout << "\n\n--------------- Printing the current cards in the Deck ---------------\n";
	int deckSize = cardsDeck->size();
	for (int i = 0; i < deckSize; i++) {
		cardsDeck->at(i).printCard();
	}
}


// Draw a card from the deck by poping the top card.
Card Deck::draw()
{
	Card topCard = cardsDeck->back();
	cardsDeck->pop_back();
	return topCard;
}



Hand::Hand()
{
}

Hand::Hand(Deck& newDeck)
{
	this->cardsHand->at(0).setParameters(0, newDeck.draw());
	this->cardsHand->at(1).setParameters(1, newDeck.draw());
	this->cardsHand->at(2).setParameters(1, newDeck.draw());
	this->cardsHand->at(3).setParameters(2, newDeck.draw());
	this->cardsHand->at(4).setParameters(2, newDeck.draw());
	this->cardsHand->at(5).setParameters(3, newDeck.draw());
}

Hand::~Hand()

{
}

void Hand::printHand()
{

	cout << "\n\n--------------- Printing the current cards in the Hand ---------------\n";
	int handSize = cardsHand->size();
	for (int i = 0; i < handSize; i++) {
		cardsHand->at(i).printHandSlot();
	}
}



HandSlot::HandSlot()
{
	this->slotValue = new int(0);
	this->slotCard = new Card(1, 1);
}


HandSlot::HandSlot(int newSlotValue, Card newSlotCard)
{
	this->slotValue = new int(newSlotValue);
	this->slotCard = &newSlotCard;
	cout << "Handslot has been initialized to: " << *slotValue << ", and card: " << slotCard->getAction() << ", " << slotCard->getGood() << endl;
}

HandSlot::~HandSlot()
{
}

void HandSlot::printHandSlot()
{
	cout << "Handslot has value: to: " << *slotValue << ", and card: " << slotCard->getAction() << ", " << slotCard->getGood() << endl;
}

void HandSlot::setParameters(int newSlotValue, Card newSlotCard)
{
	this->slotValue = new int(newSlotValue);
	this->slotCard->setParameters(newSlotCard.getGood(), newSlotCard.getAction());
	cout << "Handslot has been initialized to: " << *slotValue << ", and card: " << slotCard->getAction() << ", " << slotCard->getGood() << endl;
}