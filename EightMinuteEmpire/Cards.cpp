#include "Cards.h"
#include "Action.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>  
#include <chrono>   
#include <random>
#include "Cardsfactory.h"

using namespace std;


Card::Card()
{
	good = nullptr;
	action = nullptr;
}

Card::Card(const int myGood, const Action& myAction)
{
	good = new int(myGood);
	action = new Action(myAction);
}


Card::Card(const Card& c) {
	good = new int(*(c.good));
	action = new Action(*(c.action));
}

Card::~Card() {
	delete good;
	delete action;
}

Action* Card::getAction()
{
	return action;
}

int Card::getGood()
{
	return *good;
}



//############################### Deck Class ###############################



// Create the deck object by filling the vector with 42 cards.
Deck::Deck()
{
	CardsFactory* factory = new CardsFactory();
	// Instantiate the cardsDeck vector with new cards that have random actions and goods

	// Instantiate 15 cards with PlaceNewArmies Action
	for (int i = 0; i < 15; i++)
		cardsDeck->at(i) = factory->makeCard("PlaceNewArmiesCard");

	// Instantiate 10 cards with MoveArmies Action with land movement only
	for (int i = 15; i < 25; i++)
		cardsDeck->at(i) = factory->makeCard("MoveArmiesLandCard");

	// Instantiate 5 cards with MoveArmies Action with land & water movement
	for (int i = 25; i < 30; i++)
		cardsDeck->at(i) = factory->makeCard("MoveArmiesLandWaterCard");

	// Instantiate 5 cards with BuildCity Action
	for (int i = 30; i < 31; i++)
		cardsDeck->at(i) = factory->makeCard("BuildCityCard");

	// Instantiate 4 cards with DestoyArmy Action
	for (int i = 31; i < 39; i++)
		cardsDeck->at(i) = factory->makeCard("DestroyArmyCard");

	// Instantiate 1 cards with AND Action
	cardsDeck->at(39) = factory->makeCard("AndActionCard");

	// Instantiate 1 cards with Ignore Action
	cardsDeck->at(40) = factory->makeCard("Ignore1Card");

	// Instantiate 1 cards with Ignore Action
	cardsDeck->at(41) = factory->makeCard("Ignore2Card");
	

	cout << "Deck of cards is now instantiated and has: " << cardsDeck->size() << " cards\n";
}

Deck::~Deck()
{
}

void Deck::printDeck()
{
	int deckSize = cardsDeck->size(); // Deck size changes frequently
	cout << "\n--------------- Printing Deck with " << deckSize << " Cards ---------------\n";
	for (int i = 0; i < deckSize; i++) {
		cardsDeck->at(i)->printCard();
		cout << endl;
	}
}

// Draw a card from the deck by poping the top card.
Card* Deck::draw()
{
	Card* topCard = cardsDeck->back();
	cardsDeck->pop_back(); // Reduce the vector (deck) size by 1.
	return topCard;
}


// Shuffle the cards in the deck.
void Deck::shuffleDeck()
{
	// To obtain a time-based seed 
	cout << "Suffling Deck..." << endl;
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(cardsDeck->begin(), cardsDeck->end(), default_random_engine(seed));
	cout << "Deck has been shuffled" << endl;
}



//############################### Hand Class ###############################



Hand::Hand()
{
}

Hand::Hand(Deck* deck)
{
	for (int i = 0; i < cardsHand->size(); i++) {
		cardsHand->at(i) = new Card(*(deck->draw()));
	}
}


Hand::~Hand()
{
	for (int i = 0; i < cardsHand->size(); i++) 
	{ 
		delete cardsHand->at(i);
		cardsHand->at(i) = NULL;
	}
	cardsHand->clear(); //will remove the "placeholder memory locations"
	delete cardsHand;
}


// prints all the handSlots in the hand (their value and card) 
void Hand::printHand()
{
	cout << "\n--------------- Printing the current cards in the Hand ---------------\n";
	int handSize = cardsHand->size();
	for (int i = 0; i < handSize; i++) {
		cout << "Cost: " << getCardCostAtPosition(i + 1) << ", ";
		cardsHand->at(i)->printCard();
		cout <<  endl;
	}
}


// Get the Cost of the Card at cetrain postion in the hand (between 1 and 6)
int Hand::getCardCostAtPosition(int postion) {
	
	// Postion of the slot has to be between 1 and 6
	if (postion < 1 || postion > 6) {
		cout << "\nError: please enter a postion between 1 to 6";
		return -1;
	}

	switch (postion)
	{
	case 1:
		return 0;
	case 2:
		return 1;
	case 3:
		return 1;
	case 4:
		return 2;
	case 5:
		return 2;
	case 6:
		return 3;
	}
}

// Get the Card at cetrain postion in the hand (between 1 and 6). This also moves the rest of the cards
// backward in the hand and draws another card from the deck to be placed at the end of the hand.
Card* Hand::getCardAtPosition(int postion, Deck* deck, bool MoveCards) {

	// Postion of the slot has to be between 1 and 6
	if (postion < 1 || postion > 6) {
		cout << "\nError: please enter a postion between 1 to 6";
		return nullptr;
	}

	Card* cardptr = new Card(*(cardsHand->at(postion - 1))); // postion - 1 to because vector starts from 0
	if (MoveCards)
	moveCards(postion, deck);
	return cardptr;
}

// After a card is taken from the handslot in the hand, move the cards in the successor handslots backward
// and draw another card from the deck which will be placed at the end of the Hand (the 6th handslot). 
void Hand::moveCards(int postionOfTheRemovedCard, Deck* deck) {
	
	int postionInVector = postionOfTheRemovedCard - 1;

	// the loop ends at cardsHand->size() - 1 because we don't want to apply
	// the move to the last card, but rather we want to draw it from the deck
	for (int i = postionInVector; i < cardsHand->size() - 1; i++) {  
		cardsHand->at(i) = cardsHand->at(i + 1);
	}

	cardsHand->at(5) = new Card(*(deck->draw())); // Draw a card from the deck and place it at the last postion

	// Todo: delete the card at the postion so no memorey leak happens
	// delete cardsHand->at(postionInVector);
}

vector<Card* > Hand::cardsInHand()
{
	return *cardsHand;
}

ostream& operator<<(ostream& os, const Card& card)
{
	return os << "Card: Goods = " << *card.good << ", " <<
				  card.action->actionString(false);
}
