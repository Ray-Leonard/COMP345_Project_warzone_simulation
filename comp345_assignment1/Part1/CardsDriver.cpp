#include "Cards.h"
using namespace std;
int main()
{
	// create a deck with all kinds of cards
	Deck* deck = new Deck();
	deck->initialize();
	cout << "------The original deck--------" << endl;
	cout << *deck << endl;

	// create a hand object and fill the hand with cards up to capacity
	Hand* hand = new Hand();
	for (int i = 0; i < hand->getCapacity(); ++i)
	{
		hand->add(deck->draw());
	}
	cout << "-------Hand of cards after drawing " << hand->getCapacity() << "(hand capacity) times-------------" << endl;
	cout << *hand << endl;

	// print out the deck after drawing
	cout << "--------Deck after drawing----------" << endl;
	cout << *deck << endl;

	// play all cards in hand
	for (Card* c : hand->getHandOfCards())
	{
		cout << c->play() << endl;
		// remove the card from hand
		hand->remove(c);
		// add the card back in deck
		deck->putBack(c);
	}

	cout << "------------Hand of cards after playing--------------" << endl;
	cout << *hand << endl;

	cout << "------------Deck after playing all cards in hand-----------" << endl;
	cout << *deck << endl;

	delete deck;
	deck = nullptr;
	delete hand;
	hand = nullptr;

	return 0;
}