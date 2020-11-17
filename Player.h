#pragma once
#include <iostream>
#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

class Player
{
public:
	int name;
	vector<Territory*> territories;
    int armyPool;
	Hand* cards;
	OrdersList* orders;
	bool conquer;
	int currentPhase; // 0: initialization value, 1: startup, 2: reinforcement, 3: issue order, 4: execute

	// constructors and destructors
	Player();
	Player(int pname);
	Player(const Player& p);
	~Player(); 

	// printers and accessors
	void printTerrOwn();		// print all the territories player owns
	void printOrder();			// print all the orders player has
	int getTerrNumber();
	void setTerrPID();		//set Pid to the terrs player own
	void removeTerrByTID(int tid);		//remove the terr player own
	void addTerr(Territory* newTerr);		//add new terr to the list
	// check if player has a specific territory by ID
	bool hasTerritory(int territoryID);

	vector<Territory*> toDefend();
	vector<Territory*> toAttack(Map* map);
	// creates an order and put on player's ordersList
	// will return true if an order is successfully created, return false if no more orders are being issued
	bool issueOrder(Map* map, Deck* deck, vector<Player*>& playerList);

	Player& operator =(const Player& p);
	friend ostream& operator <<(ostream &strm, Player &player);
};