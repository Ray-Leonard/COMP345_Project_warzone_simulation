#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include <random>
#include <vector>
using namespace std;


//Default constructor
Player::Player()
{
	name = 0;
	cards = new Hand();
	armyPool = 0;
	orders = new OrdersList();
	conquer = false;
	IfNegotiate = false;
	toAdvanceTime = 20;
	currentPhase = 0;
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
	toAdvanceTime = 20;
	currentPhase = 0;
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
	for (int i = 0; i < territories.size(); i++) {
		territories.at(i)->setpId(name);
	}
}

void Player::removeTerrByTID(int tid)
{
	//find the terr has this tid
	for (int i = 0; i < territories.size(); i++) {
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
	toAdvanceTime = 20;
}

vector<Territory*> Player::toDefend(Map* map) {
	// return all the territories this player currently owns
	vector<Territory*> terrDefend;
	vector<Territory*> toAtk = toAttack(map);

	for (unsigned int i = 0; i < toAtk.size(); ++i)
	{
		// get all adjacent territories of this territory
		vector<int> adjTerr = toAtk.at(i)->getAdjacentTerritoryVec();
		// only add those do not belong to this player to the terrAttack list
		for (unsigned int j = 0; j < adjTerr.size(); ++j)
		{
			if (hasTerritory(adjTerr.at(j)))
			{
				terrDefend.push_back(map->getTerritoryById(adjTerr.at(j)));
			}
		}
	}

	return terrDefend;
}


vector<Territory*> Player::toAttack(Map* map) {
	// create a list of all neighboring territories that don't belong to this player
	vector<Territory*> terrAttack;
	for (unsigned int i = 0; i < territories.size(); ++i)
	{
		// get all adjacent territories of this territory
		vector<int> adjTerr = territories.at(i)->getAdjacentTerritoryVec();
		// only add those do not belong to this player to the terrAttack list
		for (unsigned int j = 0; j < adjTerr.size(); ++j)
		{
			if (!hasTerritory(adjTerr.at(j)))
			{
				terrAttack.push_back(map->getTerritoryById(adjTerr.at(j)));
			}
		}
	}

	return terrAttack;
}


//IssueOrder() will creat a order obj and add it to player's order list
bool Player::issueOrder(Map* map, Deck* deck, vector<Player*>& playerList)
{
	vector<Territory*> toAtk = toAttack(map);
	vector<Territory*> toDef = toDefend(map);
	// issue the deploy order first until no remaining army is in the reinforcement pool
	if (armyPool > 0)
	{
		// return a Deploy order to a random territory from toDef and assign a random number of army
		int armyToDeploy = rand() % armyPool + 1;
		int toDefendTerrIndex = rand() % toDef.size();
		armyPool -= armyToDeploy;

		orders->queue(new Deploy(this, toDef.at(toDefendTerrIndex), armyToDeploy));

		return true;
	}
	// issue advande order according to precoded toAdvanceTime times
	if (toAdvanceTime > 0)
	{
		toAdvanceTime--;
		//calling advance order--------------------------------------------------------
		int randAtk = rand() % toAtk.size();
		int randDef = rand() % toDef.size();
		Territory* source = toDef.at(randDef);
		Territory* target = toAtk.at(randAtk);
		Player* targetPlayer = nullptr;
		int targetPlayerID = target->getpId();
		// findt he target player reference
		for (unsigned int i = 0; i < playerList.size(); ++i)
		{
			if (playerList.at(i)->name == targetPlayerID)
			{
				targetPlayer = playerList.at(i);
				break;
			}
		}
		int armyToAdvance = rand() % (source->getArmyNum() + 1);
		orders->queue(new Advance(this, armyToAdvance, targetPlayer, source, target));
		return true;
	}

	// play a card if player has a card [completed logic here but need to do modifications in Cards.cpp/.h]
	if (cards->getRamainingNum() > 0)
	{
		Card* c = cards->getCard();
		// play the card that creates an order
		Order* o = c->play(this, toAtk, toDef, playerList);
		// only add the order if card is actually being played
		if (o != nullptr)
		{
			orders->queue(o);
		}

		// remove the card from hand
		cards->remove(c);
		// add the card back to deck
		deck->putBack(c);

		return true;
	}
	// if no more orders to be issued, return false
	return false;
}

//Assignment operator
Player& Player::operator=(const Player& p)
{
	if (&p != this) {
		name = p.name;
		territories = p.territories;
		cards = p.cards;
		orders = p.orders;
		IfNegotiate = p.IfNegotiate;
		toAdvanceTime = p.toAdvanceTime;
		currentPhase = p.currentPhase;
	}
	return *this;
}

//Stream insetion operator
ostream& operator<<(ostream& strm, Player& player)
{
	strm << player.name << " has " << player.territories.size() << " territories, "
		<< *(player.cards) << *(player.orders)
		<< player.armyPool << " and number of army.\n";
	return strm;
}