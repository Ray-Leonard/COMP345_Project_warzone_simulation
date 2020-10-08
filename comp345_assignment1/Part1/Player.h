#ifndef DEMO11_PLAYER_H
#define DEMO11_PLAYER_H

#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"


class Player
{
private:
	 //int counter;
public:
	string* name;
	vector<Territory*> territories;
	vector<Order*> orders;
	vector<Card*> cards;
	
	Player(); // default constructor
	Player(string pname);
	Player( Player& p, string pname);
	~Player(); // destructor

	void setTerr(vector<Territory*>);
	string getName();
	void printOrder();

	void issueOrder();
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();

	Player& operator =(const Player& p);
	friend ostream& operator <<(ostream &strm, Player &player);
};

#endif DEMO11_PLAYER_H
