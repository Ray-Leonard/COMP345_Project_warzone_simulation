#include "Player.h"
#include "Map.h"

//#include "Orders.h"
#include <iostream>
#include <ostream>
#include <typeinfo>
#include <cstring>
using namespace std;

//int counter = 0;

//Default constructor
Player::Player()
{
	name = "";
	hand = new Hand();
	army = 0;
}


//Parametric construtor set only name
Player::Player(string pname)
{
	name = pname;
	hand = new Hand();
	army = 0;
	
}

Player::~Player()
{
    delete hand;
    hand = nullptr;
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

//ToDefend() method return a list of territories
vector<Territory*> Player::toDefend() {
	// for now only arbitary territory list returned
	vector<Territory*> terrDefend;
	for (vector<Territory*>::iterator it = territories.begin(); it != territories.end(); ++it) {
		terrDefend.push_back(*it);
	}
	return terrDefend;
}
int Player::printArmy() {return army;}
//IssueOrder() will creat a order obj and add it to player's order list
void Player::issueOrder()
{	//every time make a new order obj there will be a printout to show it works
//	Order* ord = new Order();
//	orders.push_back(ord);

}

//Assignment operator
Player& Player::operator=(const Player& p)
{
	if (&p != this) {
		name = p.name;
		territories = p.territories;
		//orders = p.orders;
	}
	return *this;
}

//Stream insetipn operator
ostream& operator<<(ostream& strm, Player& player)
{
	strm << "Player "<<player.name<<" has " << player.territories.size() << " territories, "
		<< " cards and "
		<< " orders.\n";
	return strm;
}

//Player getName method to return the name of player as a string
string Player::getName()
{
	return name;
}
void Player::printTV()
{
    int k = territories.size();
    for(int i=0;i<k;i++){
        cout<<"Territory["<<territories[i]->gettId()<<"],";
    }
    cout<<endl;
}

//Player to print player's order list info
void Player::printOrder()
{
//	//print out and test if player has terr
//	cout << "The player " << this->getName() << " issues these orders: ";
//	for (int i = 0; i < this->orders.size(); i++) {
//		cout << this->orders.at(i) << "  ";
//	}
//	cout << this->orders.size() << " is the order list size\n";
}



