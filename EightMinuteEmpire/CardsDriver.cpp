/*#include <iostream>
#include "cards.h"
using namespace std;

int main() {

	// Create and print a demo card with arbitrery values.
	Card demoCard(500, 4);
	demoCard.printCard();

	// Create and print a demo deck.
	Deck demoDeck;
	demoDeck.printDeck();

	// Draw a card from the deck and print it.
	Card demoCard2 = demoDeck.draw();
	demoCard2.printCard();

	// Check the current deck after we draw a card from it.
	demoDeck.printDeck();

	// Create and print a demo Hand from the already instantiated deck.
	Hand demoHand(demoDeck);
	demoHand.printHand();

	// Check the current deck after we instantiated the Hand (we use draw 6 cards from the deck for the hand).
	demoDeck.printDeck();

	// Create and print a single handSlot object using a single card drawed from the deck.
	HandSlot newHandSlot(0, demoDeck.draw());
	newHandSlot.printHandSlot();



	system("PAUSE");

	return 0;
}*/