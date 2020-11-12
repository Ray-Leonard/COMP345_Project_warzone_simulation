#include "Player.h"
#include "Map.h"

//#include "Orders.h"
#include <iostream>
#include <ostream>
#include <typeinfo>
#include <cstring>
#include <algorithm>
using namespace std;

//int counter = 0;

//Default constructor
Player::Player()
{
	name = "";
	territories = vector<Territory*>();
	orders = new OrdersList();
	cards = new Hand();
	army = 0;
}

//Parametric construtor
Player::Player(string pname, vector<Territory*> terr, int parmy)
{
	name = pname;
	//add terr tp player
	for (vector<Territory*>::iterator it = terr.begin(); it != terr.end(); ++it) {
		territories.push_back(*it);
	}
	orders = new OrdersList();
	cards = new Hand();
	army = parmy;
}

Player::Player(string pname)
{
	name = pname;
	territories = vector<Territory*>();
	orders = new OrdersList();
	cards = new Hand();
	army = 0;
}


//copy constructor
Player::Player( Player& p)
{
	//add terr to player use deep copy
	for (unsigned int i = 0; i < p.territories.size(); i++) {
		Territory* temp_terr = new Territory(*p.territories.at(i));
		territories.push_back(temp_terr);
	}
	orders = p.orders;
	cards = p.cards;
	name = p.name;
	army = p.army;
}

//destructor
Player::~Player()
{
	for (unsigned int i = 0; i <territories.size(); i++)
	{
		delete territories.at(i);
		territories.at(i) = nullptr;
	}
	territories.clear();
	delete cards;
	cards = nullptr;
	delete orders;
	orders = nullptr;
  
}

//ToAttack() method return a list of territories
vector<Territory*> Player::toAttack(Map* map) {
	// create a list of all neighboring territories that don't belong to this player
	vector<Territory*> terrAttack;
	for (int i = 0; i < territories.size(); ++i)
	{
		// get all adjacent territories of this territory
		vector<int> adjTerr = territories.at(i)->getAdjacentTerritoryVec();
		// only add those do not belong to this player to the terrAttack list
		for (int j = 0; j < adjTerr.size(); ++j)
		{
			if (!hasTerritory(adjTerr.at(j)))
			{
				terrAttack.push_back(map->getTerritoryById(adjTerr.at(j)));
			}
		}
	}

	return terrAttack;
}

//ToDefend() method return a list of territories
vector<Territory*> Player::toDefend() {
	// return all the territories this player currently owns
	vector<Territory*> terrDefend;
	//for (vector<Territory*>::iterator it = territories.begin(); it != territories.end(); ++it) {
	//	terrDefend.push_back(*it);
	//}
	for (int i = 0; i < territories.size(); ++i)
	{
		terrDefend.push_back(territories.at(i));
	}
	return terrDefend;
}

//IssueOrder() will creat a order obj 
void Player::issueOrder(Map* map)
{	
	// 1. call toAttack() and toDefend() to get two lists of territories
	vector<Territory*> toAtk = toAttack(map);
	vector<Territory*> toDef = toDefend();

	// 2. print out the territories to defend and let the user to issue deploy order
	cout << "Territories to be Defended: " << endl;
	for (int i = 0; i < toDef.size(); ++i)
	{
		cout << i << ". " << toDef.at(i)->gettName() << "\t [Army #] " << toDef.at(i)->getArmyNum() << "\t [Continent] " << toDef.at(i)->getcName() << endl;
	}

	cout << "Please deploy your army by indicating the territory number first followed by number of armies you'd like to deploy:" << endl;
	// will only accept deploy order until all the army is being deployed
	int terrId = 0, armyNum = 0;
	while (army > 0)
	{
		while (true) {
			cin >> terrId >> armyNum;
			// validate user input
			if (terrId > toDef.size() - 1 || terrId < 0)
			{
				cout << "Invalid Territory Number! Please input again!" << endl;
				continue;
			}
			if (armyNum <= 0 || armyNum > army)
			{
				cout << "Invalid Army Number! Please input again!" << endl;
				continue;
			}
			break;
		}

		// create order based on the user input, and add the order into player's order list
		Order* deployOrder = new 

	}

}

//Assignment operator
Player& Player::operator=(const Player& p)
{
	if (&p != this) {
		name = p.name;
		for (unsigned int i = 0; i < p.territories.size(); i++) {
			Territory* temp_terr = new Territory(*p.territories.at(i));
			territories.push_back(temp_terr);
		}
		cards = p.cards;
		orders = p.orders;
	}
	return *this;
}

//Stream insetipn operator
ostream& operator<<(ostream& strm, Player& player)
{
	strm << "Player "<<player.name<<" has " << player.territories.size() << " territories, "
		<< *(player.cards) << *(player.orders)
		<< player.army <<" and number of army.\n";
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
		cout << territories[i]->gettId() << " " << territories[i]->gettName() << endl;
    }
    cout<<endl;
}

void Player::ChangePlayerArmy(int input) {army += input;}
int Player::getPlayerArmy() { return army; }
int Player::getTerrNUmber(){ return territories.size();}
vector<Territory*> Player::getTerrOwn(){return territories;}

// Adds the given order pointer to the vector of orders
void Player::AddOrderToPlayer(Order* orderToAdd) {
	orders->queue(orderToAdd);
}


bool Player::hasTerritory(int territoryID)
{
	for (int i = 0; i < getTerrNUmber(); ++i)
	{
		if (territoryID == territories.at(i)->gettId())
		{
			return true;
		}
	}
	return false;
}