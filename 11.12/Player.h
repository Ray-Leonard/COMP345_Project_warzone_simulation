#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

using namespace std;

class Player
{
public:
	string name;
	vector<Territory*> territories;
	OrdersList* orders;
	Hand* cards;
    int army;
	
	Player(); // default constructor
	Player(string pname, vector<Territory*> terr, int army);//parametic contructor
	Player(string name);
	Player( Player& p);//copy constructor
	~Player(); // destructor


	string getName();
	void printTV();
	void ChangePlayerArmy(int input);
	int getPlayerArmy();
	int getTerrNUmber();
	vector<Territory*> getTerrOwn();
	// check if player has a specific territory
	bool hasTerritory(int territoryID);

	void AddOrderToPlayer(Order* orderToAdd);

	void issueOrder(Map* map);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack(Map* map);

	Player& operator =(const Player& p);
	friend ostream& operator <<(ostream &strm, Player &player);
};