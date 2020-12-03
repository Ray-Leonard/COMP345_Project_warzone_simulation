#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include <random>
#include <vector>
using namespace std;


void Player::setStrategy(int type)
{
	if (strategy != nullptr)
	{
		delete strategy;
		strategy = nullptr;
	}
	
	if (type == Neutral)
		strategy = new NeutralPlayerStrategy(this);
	else if (type == Human)
		strategy = new HumanPlayerStrategy(this);
	else if (type == Agressive)
		strategy = new AggressivePlayerStrategy(this);
	else if (type == Benevolent)
		strategy = new BenevolentPlayerStrategy(this);
}

void Player::sortTerrIncreas()
{
	sort(territories.begin(), territories.end(),
		[]( Territory* a,  Territory* b)
		{
			return a->armyNum < b->armyNum;
		});
}


//Default constructor
Player::Player()
{
	name = 0;
	cards = new Hand();
	armyPool = 0;
	orders = new OrdersList();
	conquer = false;
	IfNegotiate = false;
	toAdvanceTime = 1;
	currentPhase = 0;
	strategy = NULL;
}

//copy constructor
Player::Player(const Player& p)
{
	//add terr to player use deep copy
	for (unsigned int i = 0; i < p.territories.size(); i++) {
		Territory* temp_terr = new Territory(*(p.territories.at(i)));
		territories.push_back(temp_terr);
	}
	// invokes the copy constructor of OrderList class
	orders = new OrdersList(*(p.orders));
	// invokes the copy constructor of Hand class
	cards = new Hand(*(p.cards));
	name = p.name;
	armyPool = p.armyPool;
	conquer = p.conquer;
	IfNegotiate = p.IfNegotiate;
	toAdvanceTime = p.toAdvanceTime;
	currentPhase = p.currentPhase;
	strategy = p.strategy;
}

//Parametric construtor set only name
Player::Player(int pname)
{
	cards = new Hand();
	armyPool = 0;
	orders = new OrdersList();
	name = pname;
	conquer = false;
	IfNegotiate = false;
	toAdvanceTime = 1;
	currentPhase = 0;
	strategy = NULL;
}

Player::~Player()
{
    delete cards;
    cards = nullptr;
	delete orders;
	orders = nullptr;
}


void Player::printTerrOwn()
{
	int k = territories.size();
	for (int i = 0; i < k; i++) {
		cout << territories[i]->gettId() << " " << territories[i]->gettName() << endl;
	}
	cout << endl;
}

void Player::printOrder()
{
	// invokes the stream insertion operator overload in ordersList class
	cout << *orders << endl;
}

int Player::getTerrNumber() 
{ 
	return territories.size(); 
}

void Player::setTerrPID()
{
	for (unsigned int i = 0; i < territories.size(); i++) {
		territories.at(i)->setpId(name);
	}
}

void Player::removeTerrByTID(int tid)
{
	//find the terr has this tid
	for (unsigned int i = 0; i < territories.size(); i++) {
		if (territories.at(i)->gettId() == tid) {
			territories.erase(territories.begin()+i);
			break;
		}
	}
}

void Player::addTerr(Territory* newTerr)
{
	territories.push_back(newTerr);
}

bool Player::hasTerritory(int territoryID)
{
	for (int i = 0; i < getTerrNumber(); ++i)
	{
		if (territoryID == territories.at(i)->gettId())
		{
			return true;
		}
	}
	return false;
}

void Player::resetToAdvanceTime()
{
	toAdvanceTime = 1;
}

void Player::setToAdvanceTime(int times)
{
	toAdvanceTime = times;
}

vector<Territory*> Player::toDefend(Map* map) {

	return this->strategy->toDefend(map);
}


vector<Territory*> Player::toAttack(Map* map) {

	return this->strategy->toAttack(map);
}


//IssueOrder() will creat a order obj and add it to player's order list
bool Player::issueOrder(Map* map, Deck* deck, vector<Player*>& playerList)
{
	return this->strategy->issueOrder(map, deck, playerList);
}

//Assignment operator
Player& Player::operator=(const Player& p)
{
	if (&p != this) {
		name = p.name;
		//add terr to player use deep copy
		for (unsigned int i = 0; i < p.territories.size(); i++) {
			Territory* temp_terr = new Territory(*(p.territories.at(i)));
			territories.push_back(temp_terr);
		}
		// invokes the copy constructor of OrderList class
		orders = new OrdersList(*(p.orders));
		// invokes the copy constructor of Hand class
		cards = new Hand(*(p.cards));

		IfNegotiate = p.IfNegotiate;
		toAdvanceTime = p.toAdvanceTime;
		currentPhase = p.currentPhase;
		strategy = p.strategy;
	}
	return *this;
}

//Stream insetion operator
ostream& operator<<(ostream& strm, Player& player)
{
	strm <<player.strategy<<" "<< player.name << " has " 
		<< player.territories.size() << " territories, "
		<< *(player.cards) << *(player.orders)
		<< player.armyPool << " and number of army.\n";
	return strm;
}