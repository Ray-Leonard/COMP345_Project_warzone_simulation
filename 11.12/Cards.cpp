#include "Cards.h"
using namespace std;

/****************Implementation for Cards class ***********************/
Card::Card()
{
	// The default card type is reinforcement
	type = CardTypes::reinforcement;
}

Card::Card(CardTypes _type)
{
	type = _type;
}

Card::~Card()
{
	// should do nothing since the only member is defined on stack
}

Card::Card(const Card& src)
{
	type = src.getType();
}

string Card::play()
{
	// 1. create special order
	string order = "A card of type [" + toString() + "] has been played.";
	// 2. remove from hand
	// 3. put back to deck
	// (the above is to be done by the game engine, for now, in the CardsDriver.cpp)
	return order;
}

CardTypes Card::getType() const
{
	return type;
}

string Card::toString()
{
	string typeName = "Undefined Type";

	switch (getType())
	{

	case CardTypes::bomb:
		typeName = "bomb";
		break;

	case CardTypes::reinforcement:
		typeName = "reinforcement";
		break;

	case CardTypes::blockade:
		typeName = "blockade";
		break;

	case CardTypes::airlift:
		typeName = "airlift";
		break;

	case CardTypes::diplomacy:
		typeName = "diplomacy";
		break;
	}

	return typeName;
}

Card& Card::operator= (const Card& src)
{
	type = src.getType();
	return *this;
}

ostream& operator<< (ostream& output, Card& card)
{

	output << card.toString() << endl;
	return output;
}





/************* Hand *************/
Hand::Hand()
{
	// we assume the capacity to be 5 if not provided by the user
	capacity = 5;
}

Hand::Hand(int _capacity)
{
	capacity = _capacity;
}

Hand::~Hand()
{
	// free the memory of the card pointers in hand
	for (unsigned int i = 0; i < handOfCards.size(); ++i)
	{
		delete handOfCards.at(i);
		handOfCards.at(i) = nullptr;
	}
	// erase everything from the hand
	handOfCards.clear();
}

Hand::Hand(const Hand& src)
{
	// copy over the capacity
	capacity = src.getCapacity();
	// copy over the cards in the src handOfCards
	for (unsigned int i = 0; i < src.getHandOfCards().size(); ++i)
	{
		// create a new card with same type as each card in the src Hand
		Card* c = new Card(src.getHandOfCards().at(i)->getType());
		// puch back the card into this Hand (no need to check capacity since src must have not exceed the capacity)
		handOfCards.push_back(c);
	}
}

void Hand::dump()
{
	cout << "The current player has the following cards in hand: " << endl;
	for(unsigned int i = 0; i < handOfCards.size(); ++i)
	{
		cout << *handOfCards.at(i);
	}
}

bool Hand::add(Card* card)
{
	// only can add a card when it does not reach the capacity
	if (handOfCards.size() < capacity)
	{
		handOfCards.push_back(card);
		return true;
	}
	return false;
}

bool Hand::remove(Card* card)
{
	// only delete a card when 1. the capacity > 0 and 2. there is such a card that matches the parameter
	if (handOfCards.size() > 0)
	{
		// traverse through the handOfCards to find a matching card with the param card
		for (unsigned int i = 0; i < handOfCards.size(); ++i)
		{
			// here we compare the reference, because we want to find the exact card, not any card with same type
			if (card == handOfCards.at(i))
			{
				handOfCards.erase(handOfCards.begin() + i);
				return true;
			}
		}
	}
	return false;
}

int Hand::getCapacity() const
{
	return capacity;
}

vector<Card*> Hand::getHandOfCards() const
{
	return handOfCards;
}

Hand& Hand::operator = (const Hand& src)
{
	// copy over the capacity
	capacity = src.getCapacity();
	// clear the handOfCards for this object, since it may contain other cards
	handOfCards.clear();
	// copy over the cards in the src handOfCards
	for (unsigned int i = 0; i < src.getHandOfCards().size(); ++i)
	{
		// create a new card with same type as each card in the src Hand
		Card* c = new Card(src.getHandOfCards().at(i)->getType());
		// puch back the card into this Hand (no need to check capacity since src must have not exceed the capacity)
		handOfCards.push_back(c);
	}

	return *this;
}

ostream& operator << (ostream& output, Hand& hand)
{
	output << "The current player has the following cards in hand: " << endl;
	for (unsigned int i = 0; i < hand.getHandOfCards().size(); ++i)
	{
		output << *hand.getHandOfCards().at(i);
	}

	return output;
}






/********** Deck ***************/

Deck::Deck()
{
	// assume the initial capacity of a deck is 30
	capacity = 30;
	cout << "A deck with 30 cards has been created!" << endl;
}

Deck::Deck(int _capacity)
{
	capacity = _capacity;
}

Deck::~Deck()
{
	// free the memory of the card pointers in deck
	for (unsigned int i = 0; i < deckOfCards.size(); ++i)
	{
		delete deckOfCards.at(i);
		deckOfCards.at(i) = nullptr;
	}
	// erase everything from the hand
	deckOfCards.clear();
}

Deck::Deck(const Deck& src)
{
	capacity = src.getCapacity();
	// copy over all the cards in the src Deck
	for (unsigned int i = 0; i < src.getDeckOfCards().size(); ++i)
	{
		Card* c = new Card(src.getDeckOfCards().at(i)->getType());
		deckOfCards.push_back(c);
	}
}

void Deck::initialize()
{
	// fill the deck with #capacity many of cards, one kind at a time
	for (int i = 0; i < capacity; ++i)
	{
		Card* c = new Card(static_cast<CardTypes>(i % 6));
		deckOfCards.push_back(c);
	}
}

Card* Deck::draw()
{
	// only perform the draw if the deck still has cards remaining
	if(deckOfCards.size() > 0)
	{
		// shuffle the deck
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		shuffle(deckOfCards.begin(), deckOfCards.end(), default_random_engine(seed));
		// since the deck is already randomized, we draw from the top.
		Card* c = deckOfCards.at(0);
		// delete this first card from the deck
		deckOfCards.erase(deckOfCards.begin());
		// return the card
		return c;
	}
	// when there are no more cards to draw, return null pointer
	return nullptr;
}

void Deck::putBack(Card* card)
{
	// put the card in the end of the deck
	deckOfCards.push_back(card);
}

int Deck::getCapacity() const
{
	return capacity;
}

vector<Card*> Deck::getDeckOfCards() const
{
	return deckOfCards;
}

Deck& Deck::operator = (const Deck& src)
{
	// copy over the capacity
	capacity = src.getCapacity();
	// clear the deckOfCards for this object, since it may contain other cards
	deckOfCards.clear();
	// copy over the cards in the src deckOfCards
	for (unsigned int i = 0; i < src.getDeckOfCards().size(); ++i)
	{
		// create a new card with same type as each card in the src deck
		Card* c = new Card(src.getDeckOfCards().at(i)->getType());
		// puch back the card into this deck (no need to check capacity since src must have not exceed the capacity)
		deckOfCards.push_back(c);
	}
	return *this;
}

ostream& operator << (ostream& output, Deck& deck)
{
	output << "The deck has the following cards: " << endl;
	for (unsigned int i = 0; i < deck.getDeckOfCards().size(); ++i)
	{
		output << i + 1 << ". " << *deck.getDeckOfCards().at(i);
	}
	return output;
}
