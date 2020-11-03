#ifndef untitled_PLAYER_H
#define untitled_PLAYER_H

#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include "Map.h"
#include "Cards.h"
//#include "Orders.h"


class Player
{
private:
	 //int counter;
public:
	string name;
	vector<Territory*> territories;
	//vector<Order*> orders;
    int army;
	Hand* hand;
	
	Player(); // default constructor
	Player(string pname);
	//Player( Player& p, string pname);
	~Player(); // destructor


	string getName();
	void printTV();
	void printOrder();
	int printArmy();

	void issueOrder();
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();

	Player& operator =(const Player& p);
	friend ostream& operator <<(ostream &strm, Player &player);
};

#endif 