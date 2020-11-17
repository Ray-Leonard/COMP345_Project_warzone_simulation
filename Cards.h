#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "Orders.h"
#include "Map.h"

class Player;

using namespace std;

enum class CardTypes {bomb, blockade, airlift, diplomacy};

/* The Card class represents a single card
   Both Deck and Hand should have a collection of Card.
 */
class Card
{
private:
	// Each card must have a type
	CardTypes type;
public:
	// default constructor	
	Card();
	// pramartized constructor
	Card(CardTypes _type);
	// destructor
	~Card();
	// copy constructor
	Card(const Card& src);
	/* play method that is called to play the card, which creates
	   an order and adds it to the player's list of orders and then returns this card to the deck.
	   For now the return type is string, but should be Order. */
	Order* play(Player* player, vector<Territory*>& toAtk, vector<Territory*>& toDef, vector<Player*>& playerList);
	// getter to get the type of the card
	CardTypes getType() const;
	// convert the type to string
	string toString();
	

	// copy assignment
	Card& operator = (const Card& src);
	// stream insertion operator overload
	friend ostream& operator << (ostream& output, Card& deck);

};



/* Each player has a hand of cards */
class Hand
{
private:
	// maximun number of card that can be hold by a player
	int capacity;
	// collection of cards owned by the player
	vector<Card*> handOfCards;
public:
	// constructors
	Hand();
	Hand(int _capacity);
	// destructor
	~Hand();
	// copy constructor
	Hand(const Hand& src);
	// print out all the cards in the collection
	void dump();
	// add/remove card functions
	bool add(Card* card);
	bool remove(Card* card);
	// getter
	int getRamainingNum();
	Card* getCard();
	int getCapacity() const;
	vector<Card*> getHandOfCards() const;

	// operator overloading
	// copy assignment
	Hand& operator = (const Hand& src);
	// stream insertion operator overload
	friend ostream& operator << (ostream& output, Hand& hand);
};

/* The deck of cards in the game
   There should only be one Deck for a game
 */
class Deck
{
private:
	// Total number of cards possible in a deck
	int capacity;
	// Collection of cards
	vector<Card*> deckOfCards;
public:
	// constructors
	Deck();
	Deck(int _capacity);
	// destructor
	~Deck();
	// copy constructor
	Deck(const Deck& src);
	// initialize method that will fill the deck with a defined number of cards according to their weights
	void initialize();
	// draw method that allows a player to draw a card at random
	// from the cards remaining in the deck and place in their hand
	// Return the card drawn. If no more cards left in the deck, return nullptr
	// Take in a Hand as parameter to indicate which hand to add card to
	Card* draw();
	// accepting an card being put back to the deck
	void putBack(Card* card);
	// getter
	int getCapacity()const;
	vector<Card*> getDeckOfCards()const;

	// copy assignment
	Deck& operator = (const Deck& src);
	// stream insertion operator overload
	friend ostream& operator << (ostream& output, Deck& deck);
};

#endif