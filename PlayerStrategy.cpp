#pragma once
#include "PlayerStrategy.h"
#include "Player.h"
using namespace std;

//******************************Player

PlayerStrategy::PlayerStrategy() { player=nullptr; }

PlayerStrategy::PlayerStrategy(Player* thisPlayer):player(thisPlayer){}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& toCopy) { player = toCopy.player; }

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& rightSide) { player = rightSide.player; return *this; }

PlayerStrategy::~PlayerStrategy() {}


ostream& operator<<(std::ostream& out, const PlayerStrategy& toOutput)
{
	out << "Player Strategy" << endl;
	return out;
}

//***********************************AggressivePlayer
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* thisplayer) { player = thisplayer; }

//AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& rightSide) { PlayerStrategy::operator=(rightSide); return *this; }

AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

vector<Territory*> AggressivePlayerStrategy::toDefend(Map* map)
{
	this->player->sortTerrIncreas();
	return player->territories;
}

vector<Territory*> AggressivePlayerStrategy::toAttack(Map* map)
{
	// create a list of all terr player not own
	//so aggressive player only attack Terrs not owns
	vector<Territory*> terrAttack;
	for (unsigned int i = 0; i < player->territories.size(); ++i)
	{
		// get all adjacent territories of this territory
		vector<int> adjTerr = player->territories.at(i)->getAdjacentTerritoryVec();
		// only add those do not belong to this player to the terrAttack list
		for (unsigned int j = 0; j < adjTerr.size(); ++j)
		{
			if (!player->hasTerritory(adjTerr.at(j)))
			{
				terrAttack.push_back(map->getTerritoryById(adjTerr.at(j)));
			}
		}
	}
	return terrAttack;
}

bool AggressivePlayerStrategy::issueOrder(Map* map, Deck* deck, vector<Player*>& playerList)
{
	vector<Territory*> toDef = toDefend(map);
	vector<Territory*> toAtk = toAttack(map);
	

	// issue the deploy order first until no remaining army is in the reinforcement pool
	if (player->armyPool > 0)
	{
		// return a Deploy order to a random territory from toDef and assign a random number of army
		int armyToDeploy = player->armyPool;
		//int toDefendTerrIndex = rand() % toDef.size();
		player->armyPool -= armyToDeploy;

		player->orders->queue(new Deploy(player, toDef.at(toDef.size()-1), armyToDeploy));
		//player only delopy to fist toDef terr. Each term only one terr got new armies

		return true;
	}
	 //issue advande order
	if (player->toAdvanceTime > 0)
	{
		player->toAdvanceTime--;
		//calling advance order--------------------------------------------------------

		int randDef = rand() % toDef.size();
		
		for (unsigned int i = 0; i < player->territories.size(); i++)
		{
			int randAtk = rand() % toAtk.size();
			if (player->territories.at(i)->armyNum != 0)
			{
				Territory* source = player->territories.at(i);
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
				int armyToAdvance = source->getArmyNum();

				player->orders->queue(new Advance(player, armyToAdvance, targetPlayer, source, target));
			}
		}

		return true;
	}
	

	// play a card if player has a card [completed logic here but need to do modifications in Cards.cpp/.h]
	if (player->cards->getRamainingNum() > 0)
	{
		Card* c = player->cards->getCard(0);
		// play the card that creates an order
		Order* o = c->play(player, toAtk, toDef, playerList);
		// only add the order if card is actually being played
		if (o != nullptr)
		{
			player->orders->queue(o);
		}

		// remove the card from hand
		player->cards->remove(c);
		// add the card back to deck
		deck->putBack(c);

		return true;
	}
		// if no more orders to be issued, return false
	return false;
}

ostream& operator<<(std::ostream& out, const AggressivePlayerStrategy& toOutput)
{
	out << "Aggressive Player" << endl;
	return out;
}

//*****************************BenevolentPlayer
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* thisplayer) { player = thisplayer; }
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

vector<Territory*> BenevolentPlayerStrategy::toDefend(Map* map)
{
	this->player->sortTerrIncreas();
	return player->territories;
}

vector<Territory*> BenevolentPlayerStrategy::toAttack(Map* map)
{
	//Benevolent player wont attack anyone, so this funtion only return the Terr player has
	vector<Territory*> terrAttack;
	for (unsigned int i = 0; i < player->territories.size(); i++) 
	{
		terrAttack.push_back(player->territories.at(i));
	}
	return terrAttack;
}

bool BenevolentPlayerStrategy::issueOrder(Map* map, Deck* deck, vector<Player*>& playerList)
{

	vector<Territory*> toAtk = toAttack(map);//all Terr belong to player
	vector<Territory*> toDef = toDefend(map);//weakest Terr
	// issue the deploy order first until no remaining army is in the reinforcement pool
	if (player->armyPool > 0)
	{
		// return a Deploy order to the strongest territory from toDef and assign all number of army
		int armyToDeploy = player->armyPool;
		player->orders->queue(new Deploy(player, toDef.at(0), armyToDeploy));
		player->armyPool -= armyToDeploy;

		return true;
	}
	// issue advande order according to precoded toAdvanceTime times
	if (player->toAdvanceTime > 0)
	{
		player->toAdvanceTime--;
		//calling advance order--------------------------------------------------------
		// target terr is the weakest terr
		Territory* target = toDef.at(0);
		// find a list of terr adj to target that belongs to the player
		vector<Territory*> sourceTerrList;
		// get all adjacent territories of this territory
		vector<int> adjTerr = target->getAdjacentTerritoryVec();
		// only add those belong to this player to the sourceTerrlist
		for (unsigned int j = 0; j < adjTerr.size(); ++j)
		{
			if (player->hasTerritory(adjTerr.at(j)))
			{
				sourceTerrList.push_back(map->getTerritoryById(adjTerr.at(j)));
			}
		}

		// issue advance order based on sourceTerrList
		for (unsigned int i = 0; i < sourceTerrList.size(); ++i)
		{
			Territory* source = sourceTerrList.at(i);
			int armyToAdvance = rand() % (source->getArmyNum() + 1);
			player->orders->queue(new Advance(player, armyToAdvance, player, source, target));
		}
		return true;
	}


	// play a card if player has a card
	if (player->cards->getRamainingNum() > 0)
	{
		Card* c = player->cards->getCard(0);
		// play the card that creates an order
		Order* o = c->play(player, toAtk, toDef, playerList);
		// only add the order if card is actually being played
		if (o != nullptr)
		{
			player->orders->queue(o);
		}

		// remove the card from hand
		player->cards->remove(c);
		// add the card back to deck
		deck->putBack(c);

		return true;
	}
	// if no more orders to be issued, return false
	return false;
}

ostream& operator<<(std::ostream& out, const BenevolentPlayerStrategy& toOutput)
{
	out << "Benevolent Player" << endl;
	return out;
}

//****************************Humanplayer

HumanPlayerStrategy::HumanPlayerStrategy(Player* thisplayer) { player = thisplayer; }
HumanPlayerStrategy::~HumanPlayerStrategy() {}

vector<Territory*> HumanPlayerStrategy::toDefend(Map* map)
{
	// simply return all the territory that this player has

	return player->territories;
}

vector<Territory*> HumanPlayerStrategy::toAttack(Map* map)
{
	// create a list of all neighboring territories that don't belong to this player
	vector<Territory*> terrAttack;
	for (unsigned int i = 0; i < player->territories.size(); ++i)
	{
		// get all adjacent territories of this territory
		vector<int> adjTerr = player->territories.at(i)->getAdjacentTerritoryVec();
		// only add those do not belong to this player to the terrAttack list
		for (unsigned int j = 0; j < adjTerr.size(); ++j)
		{
			if (!player->hasTerritory(adjTerr.at(j)))
			{
				bool isRedundent = false;
				for (unsigned int k = 0; k < terrAttack.size(); ++k)
				{
					if (adjTerr.at(j) == terrAttack.at(k)->tID)
					{
						isRedundent = true;
						break;
					}
				}

				if(!isRedundent)
					terrAttack.push_back(map->getTerritoryById(adjTerr.at(j)));
			}
		}
	}
	return terrAttack;
}

bool HumanPlayerStrategy::issueOrder(Map* map, Deck* deck, vector<Player*>& playerList)
{
	vector<Territory*> toDef = toDefend(map);
	vector<Territory*> toAtk = toAttack(map);
	
	// issue the deploy order first until no remaining army is in the reinforcement pool
	if (player->armyPool > 0)
	{
		// print out player's all territories
		cout << "-------You can deploy to the following territories-------" << endl;
		cout << "Index\t" << "Territory\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
		for (unsigned int i = 0; i < player->territories.size(); ++i)
		{
			cout << "[" << i << "]. " << *player->territories.at(i) << endl;
		}

		cout << "You now have: " << player->armyPool << " armies to deploy." << endl;
		cout << "Please input the territory index number followed by number of armies to deploy: " << endl;

		int armyToDeploy = 0;
		int toDeployTerrIndex = rand() % toDef.size();
		
		// get the user input
		cin >> toDeployTerrIndex >> armyToDeploy;

		// check the user input
		if (armyToDeploy > player->armyPool || toDeployTerrIndex < 0 || toDeployTerrIndex >= (int)toDef.size())
		{
			cout << "Invalid numbers, please make sure the number is correct!" << endl;
			return true;
		}

		// once the user input is verified, issue the deploy order
		player->armyPool -= armyToDeploy;
		player->orders->queue(new Deploy(player, player->territories.at(toDeployTerrIndex), armyToDeploy));

		return true;
	}


	// issue advande order according to precoded toAdvanceTime times
	if (player->toAdvanceTime > 0)
	{
		player->toAdvanceTime--;
		// print out the territory to attack and territory to defend
		cout << "------These are the territories to [Attack]--------" << endl;
		cout << "Territory" << ":";
		cout << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
		for (unsigned int i = 0; i < toAtk.size(); ++i)
		{
			cout << *toAtk.at(i);
		}
		cout << "------These are the territories to [Defend]--------" << endl;
		cout << "Territory" << ":";
		cout << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
		for (unsigned int i = 0; i < toDef.size(); ++i)
		{
			cout << *toDef.at(i);
		}
		// ask for user input until the user do not wish to continue issuing advance orders
		while (true)
		{
			// then ask the user if he wish to continue issuing orders
			cout << "Do you wish to issue any advance order?(y/Y)" << endl;
			char c = 'n';
			cin >> c;
			if ((c != 'y') && (c != 'Y'))
			{
				break;
			}
			
			cout << "Please specify the following in order to create an advance order: " << endl;
			cout << "Source Territory ID (from toDefend list): ";
			int sourceID = 0;
			cin >> sourceID;
			// identify the source territory reference by its ID
			Territory* source = map->getTerritoryById(sourceID);

			cout << "Target Territory ID (from either toDefend or toAttack list): ";
			int targetID = 0;
			cin >> targetID;
			// identify the target territory reference by its ID
			Territory* target = map->getTerritoryById(targetID);

			cout << "Number of army to Advance (no more than source terr army): ";
			int armyToAdvance = 0;
			cin >> armyToAdvance;

			// identify the target player
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

			// finally issue the order
			player->orders->queue(new Advance(player, armyToAdvance, targetPlayer, source, target));
		}
		return true;
	}

	// play a card if player has a card [completed logic here but need to do modifications in Cards.cpp/.h]
	if (player->cards->getRamainingNum() > 0)
	{
		// print out all the cards 
		cout << "You have the following cards in hand: " << endl;
		cout << *player->cards;

		// ask for user input
		cout << "If you wish to play any, please input the index. If you do not wish to play, please input -1: ";
		int index = -1;
		cin >> index;

		if (index == -1 || index < 0 || index >= player->cards->getRamainingNum())
		{
			return false;
		}

		// then play the card************************************************
		Card* c = player->cards->getCard(index);
		Order* o = nullptr;
		// create bomb order--------------------------------------------
		if (c->getType() == CardTypes::bomb)
		{
			// print out all the toAtk territories
			cout << "---The following are the territories you can bomb:---" << endl;
			cout << "Territory:";
			cout << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
			for (unsigned int i = 0; i < toAtk.size(); ++i)
			{
				cout << *toAtk.at(i);
			}
			cout << "Please input the Target Territory ID: ";
			int targetID = 0;
			cin >> targetID;
			// identify the target territory reference by its ID
			Territory* target = map->getTerritoryById(targetID);
			o = new Bomb(player, nullptr, target);
		}
		// create a blockade order-----------------------------------------------------
		else if (c->getType() == CardTypes::blockade)
		{
			cout << "---The following are the territories you can blocade:---" << endl;
			cout << "Territory" << ":";
			cout << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
			for (unsigned int i = 0; i < toDef.size(); ++i)
			{
				cout << *toDef.at(i);
			}
			cout << "Please input the Target Territory ID: ";
			int targetID = 0;
			cin >> targetID;
			// identify the target territory reference by its ID
			Territory* target = map->getTerritoryById(targetID);
			o = new Blockade(player, target);
		}
		// create an airlift order------------------------------------------------
		else if (c->getType() == CardTypes::airlift)
		{
			cout << "------These are the territories to [Attack]--------" << endl;
			cout << "Territory" << ":";
			cout << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
			for (unsigned int i = 0; i < toAtk.size(); ++i)
			{
				cout << *toAtk.at(i);
			}
			cout << "------These are the territories to [Defend]--------" << endl;
			cout << "Territory" << ":";
			cout << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
			for (unsigned int i = 0; i < toDef.size(); ++i)
			{
				cout << *toDef.at(i);
			}
			cout << "Please specify the following in order to create an advance order: " << endl;
			cout << "Source Territory ID (from toDefend list): ";
			int sourceID = 0;
			cin >> sourceID;
			// identify the source territory reference by its ID
			Territory* source = map->getTerritoryById(sourceID);

			cout << "Target Territory ID (from either toDefend or toAttack list): ";
			int targetID = 0;
			cin >> targetID;
			// identify the target territory reference by its ID
			Territory* target = map->getTerritoryById(targetID);

			cout << "Number of army to Airlift (no more than source terr army): ";
			int armyToAirlift = 0;
			cin >> armyToAirlift;

			// identify the target player
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
			o =  new Airlift(player, armyToAirlift, targetPlayer, source, target);
		}
		// create a negotiate order----------------------------------------------------
		else if (c->getType() == CardTypes::diplomacy)
		{
			Player* targetPlayer = player;
			cout << "The following are the players you can negotiate with: " << endl;
			for (unsigned int i = 0; i < playerList.size(); ++i)
			{
				if (playerList.at(i) != player)
				{
					cout << "[" << i << "]. Player " << playerList.at(i)->name;
				}
			}

			cout << "Please input the player index:";
			int playerIndex = 0;
			cin >> playerIndex;

			o = new Negotiate(player, playerList.at(playerIndex), nullptr);
		}

		// only add the order if card is actually being played
		if (o != nullptr)
		{
			player->orders->queue(o);
		}

		// remove the card from hand
		player->cards->remove(c);
		// add the card back to deck
		deck->putBack(c);

		return true;
	}
	// if no more orders to be issued, return false
	return false;
}

ostream& operator<<(std::ostream& out, const HumanPlayerStrategy& toOutput)
{
	out << "Human player" << endl;
	return out;
}

//*************************************NeutralPlayer
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* thisplayer) { player = thisplayer; }
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

vector<Territory*> NeutralPlayerStrategy::toDefend(Map* map)
{
	return vector<Territory*>();
}

vector<Territory*> NeutralPlayerStrategy::toAttack(Map* map)
{
	return vector<Territory*>();
}

bool NeutralPlayerStrategy::issueOrder(Map* map, Deck* deck, vector<Player*>& playerList)
{
	return false;
}

ostream& operator<<(std::ostream& out, const NeutralPlayerStrategy& toOutput)
{
	out << "Neutral player" << endl;
	return out;
}
