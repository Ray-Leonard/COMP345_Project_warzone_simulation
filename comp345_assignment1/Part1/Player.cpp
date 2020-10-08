#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include <iostream>
#include <ostream>
#include <typeinfo>
#include <cstring>
using namespace std;

int counter = 0;

//Default constructor
Player::Player()
{
	name = new string("");
}

//Destructor delete new name vireable which not using
Player::~Player()
{
	delete name;
	cout << "Player obj deleted\n";
}
//Parametric construtor set only name
Player::Player(string pname)
{
	name = new string("");
	*name = pname;
}

//Copy constructor with a new name
Player::Player( Player& p ,string pname)
{
	for (vector<Territory*>::iterator it = p.territories.begin(); it != p.territories.end(); ++it) {
		this->territories.push_back(*it);
	}
	name = new string("");
	*name = pname;
	
	for (vector<Order*>::iterator it = p.orders.begin(); it != p.orders.end(); ++it) {
		this->orders.push_back(*it);
	}

	for (vector<Card*>::iterator it = p.cards.begin(); it != p.cards.end(); ++it) {
		this->cards.push_back(*it);
	}

}

//ToAttack() method return a list of territories
vector<Territory*> Player::toAttack() {
	// for now only arbitary territory list returned
	vector<Territory*> terrAttack;
	for (vector<Territory*>::iterator it = territories.begin(); it != territories.end(); ++it) {
		terrAttack.push_back(*it);
	}
	return terrAttack;
}

//IssueOrder() will creat a order obj and add it to player's order list
void Player::issueOrder()
{	//every time make a new order obj there wu=ill be a printout to show it works
	Order* ord = new Order();
	orders.push_back(ord);
}

//ToDefend() method return a list of territories
vector<Territory*> Player::toDefend() {
	// for now only arbitary territory list returned
	vector<Territory*> terrDefend;
	for (vector<Territory*>::iterator it = territories.begin(); it != territories.end(); ++it) {
		terrDefend.push_back(*it);
	}
	return terrDefend;
}

//Assignment operator
Player& Player::operator=(const Player& p)
{
	if (&p != this) {
		name = p.name;
		territories = p.territories;
		orders = p.orders;
	}
	return *this;
}

//Stream insetipn operator
ostream& operator<<(ostream& strm, Player& player)
{
	strm << "Player "<<*player.name<<" has " << player.territories.size() << " territories, "
		<< player.cards.size() << " cards and "
		<< player.orders.size() << " orders.\n";
	return strm;
}

//Player getName method to return the name of player as a string
string Player::getName()
{
	return *name;
}

//Player SetTerr method to set a list of territories to the player
void Player::setTerr(vector<Territory*> p)
{
	for (vector<Territory*>::iterator it = p.begin(); it != p.end(); ++it) {
		this->territories.push_back(*it);
	}
	//print out
	cout << "The player " << this->getName() << " owns these territorties: ";
	for (int i = 0; i < this->territories.size(); i++) {
		cout << this->territories.at(i)->getOwnership() << "  ";
	}
	cout << "\n";
}

//Player to print player's order list info
void Player::printOrder()
{
	//print out and test if player has terr
	cout << "The player " << this->getName() << " issues these orders: ";
	for (int i = 0; i < this->orders.size(); i++) {
		cout << this->orders.at(i) << "  ";
	}
	cout << this->orders.size() << " is the order list size\n";
}
