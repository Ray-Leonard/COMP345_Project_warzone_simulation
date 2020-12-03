#pragma once
#include<iostream>
#include "Map.h"
#include "Player.h"
#include "Orders.h"
using namespace std;

class Player;

class PlayerStrategy {
public:
	Player* player;
	PlayerStrategy();
	PlayerStrategy(Player* thisPlayer);
	PlayerStrategy(const PlayerStrategy& toCopy);
	PlayerStrategy& operator=(const PlayerStrategy& rightSide);
	virtual ~PlayerStrategy();
	
	virtual vector<Territory*> toDefend(Map* map) = 0;
	virtual vector<Territory*> toAttack(Map* map) = 0;
	virtual bool issueOrder(Map* map, Deck* deck, vector<Player*>& playerList) = 0;
	friend ostream& operator<<(std::ostream& out, const PlayerStrategy& toOutput);

};


class AggressivePlayerStrategy : public PlayerStrategy {
public:
	//Player* player;
	AggressivePlayerStrategy(Player* player);
	//AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& rightSide);
	~AggressivePlayerStrategy();

	vector<Territory*> toDefend(Map* map);
	vector<Territory*> toAttack(Map* map);
	bool issueOrder(Map* map, Deck* deck, vector<Player*>& playerList);
	friend ostream& operator<<(std::ostream& out, const AggressivePlayerStrategy& toOutput);

};

class  BenevolentPlayerStrategy : public PlayerStrategy {
public:
	//Player* player;
	BenevolentPlayerStrategy(Player* player);
	~BenevolentPlayerStrategy();

	vector<Territory*> toDefend(Map* map);
	vector<Territory*> toAttack(Map* map);
	bool issueOrder(Map* map, Deck* deck, vector<Player*>& playerList);
	friend ostream& operator<<(std::ostream& out, const BenevolentPlayerStrategy& toOutput);

};

class HumanPlayerStrategy : public PlayerStrategy {
public:
	//Player* player;
	HumanPlayerStrategy(Player* player);
	~HumanPlayerStrategy();

	vector<Territory*> toDefend(Map* map);
	vector<Territory*> toAttack(Map* map);
	bool issueOrder(Map* map, Deck* deck, vector<Player*>& playerList);
	friend ostream& operator<<(std::ostream& out, const HumanPlayerStrategy& toOutput);

};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
	//Player* player;
	NeutralPlayerStrategy(Player* player);
	~NeutralPlayerStrategy();

	vector<Territory*> toDefend(Map* map);
	vector<Territory*> toAttack(Map* map);
	bool issueOrder(Map* map, Deck* deck, vector<Player*>& playerList);
	friend ostream& operator<<(std::ostream& out, const NeutralPlayerStrategy& toOutput);

};