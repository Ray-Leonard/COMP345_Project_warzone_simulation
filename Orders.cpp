#include <algorithm>
#include "Orders.h"
#include <vector>
#include<iostream>
#include "Player.h"
using namespace std;


// ----------------Order-----------------------------
Order::Order() :
     player(nullptr), targetTerritory(nullptr), wasExecuted(false), isEnabled(true){}

Order::Order(Player* player, Territory* target):
     player(player), targetTerritory(target), wasExecuted(false), isEnabled(true) {}

Order::Order(const Order& toCopy) {
    player = toCopy.player;
    wasExecuted = toCopy.wasExecuted;
    isEnabled = toCopy.isEnabled;
    targetTerritory = toCopy.targetTerritory;
}

Order::~Order() 
{ 
    // do nothing: it should not delete the player since the player is still playing
    // player is only to be deleted by gameCheck()
}

Order& Order::operator=(const Order& rightSide) {
    player = rightSide.player;
    wasExecuted = rightSide.wasExecuted;
    isEnabled = rightSide.isEnabled;
    targetTerritory = rightSide.targetTerritory;
    return *this;
}

// an order must be enabled and not executed to be valid
bool Order::validate() { return (isEnabled && !wasExecuted); }

void Order::disableOrder() { isEnabled = false; }

Player* Order::getPlayer() { return player; }

Territory* Order::getTargetTerritory() { return targetTerritory; }

void Order::PrintMsg() const{
    cout << *this;
}

std::ostream& operator<<(ostream& out, const Order& toOutput) {
    out << "This order is issued by: " << toOutput.player->name << endl;
    return out;
}


//-------------------Deploy-------------------------------
Deploy::Deploy() : Order(), numberOfArmies(0) {}

Deploy::Deploy(Player* player, Territory* territory, int requestedNumberOfArmies) :
    Order(player, territory), numberOfArmies(requestedNumberOfArmies) {
}

Deploy::Deploy(const Deploy& toCopy) : Order(toCopy) {
    numberOfArmies = toCopy.numberOfArmies;
}

Deploy& Deploy::operator=(const Deploy& rightSide) {
    Order::operator=(rightSide);
    numberOfArmies = rightSide.numberOfArmies;
    return *this;
}

Deploy::~Deploy() {
    // do nothing
}

bool Deploy::validate() 
{
    // check if the target terr belongs to the player
    return (Order::validate() && player->hasTerritory(targetTerritory->gettId()));
 }

string Deploy::execute() {
    if (!validate()) {
        // when this order cannot be executed, the army deployed should be returned to the player
        // but this should not really be happening because player only deploy to its own territory returned by toDefend()
        player->armyPool += numberOfArmies;
        return "";
    }
    wasExecuted = true;
    // add army to the territory
    targetTerritory->addArmy(numberOfArmies);

    // output a message to console indicating the effect of this order
    //cout << *this;
    return "";
}

const int Deploy::getPriority() const
{
    return orderPriority;
}

void Deploy::PrintMsg() const {
    cout << *this;
}

ostream& operator<<(std::ostream& out, const Deploy& toOutput)
{
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Deploy Order*" << " deploye " << toOutput.numberOfArmies
            << " army to [" << toOutput.targetTerritory->gettName() << "]" << endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Deploy* Player [" << toOutput.player->name << "] has deployed " 
                << toOutput.numberOfArmies<< " army to [" 
                << toOutput.targetTerritory->gettName() << "] now with army# " 
                << toOutput.targetTerritory->getArmyNum() << endl;
        }
        else
        {
            out << "*Deploy* order of player [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}


//----------------Advance-----------------------
Advance::Advance() : Order(), numberOfArmies(0),sourceTerritory(nullptr), targetPlayer(nullptr) {}

Advance::Advance(Player* player, int advanceArmies, Player* targetPlayer,Territory* sourceTerritory,
    Territory* targetTerritory)
    : Order(player, targetTerritory), numberOfArmies(advanceArmies){
    this->sourceTerritory = sourceTerritory;
    this->targetPlayer = targetPlayer;
}

Advance::Advance(const Advance& toCopy) : Order(toCopy) {
    numberOfArmies = toCopy.numberOfArmies;
    sourceTerritory = toCopy.sourceTerritory;
    targetPlayer = toCopy.targetPlayer;
}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& rightSide) {
    Order::operator=(rightSide);
    numberOfArmies = rightSide.numberOfArmies;
    sourceTerritory = rightSide.sourceTerritory;
    targetPlayer = rightSide.targetPlayer;
    return *this;
}

bool Advance::validate(){
    // Check if source and target territories are neighbors
    //a list having all adjacent territories belong to the source territory
    vector<int> adjTerr = sourceTerritory->getAdjacentTerritoryVec();
    bool OneOfAdjTerrl=false;

    for (unsigned int i = 0; i < adjTerr.size(); ++i)
    {
        if (targetTerritory->gettId() != adjTerr.at(i))
        {
            OneOfAdjTerrl = false;
        }
        else {
            OneOfAdjTerrl = true;
            break;
        }
    }

    bool isTargetValid = false;
    if (targetTerritory->getpId() == 0)
    {
        isTargetValid = true;
    }
    else if (targetPlayer == nullptr)
    {
        isTargetValid = false;
    }
    else
    {
        isTargetValid = targetPlayer->name == targetTerritory->getpId();
    }

    return (Order::validate() && OneOfAdjTerrl
        && sourceTerritory->getArmyNum() != 0
        && sourceTerritory->getpId() != 0
        && numberOfArmies != 0
        && sourceTerritory->getpId() == player->name
        && isTargetValid);
}

string Advance::execute() {
    if (!validate()) {
        //cout << *this;
        return "";
    }
    wasExecuted = true;
    if (sourceTerritory->getArmyNum() < numberOfArmies)
    {
        numberOfArmies = sourceTerritory->getArmyNum();
    }
    //player will remove number of army from source terr
    sourceTerritory->setArmy(sourceTerritory->getArmyNum() - numberOfArmies);
    //cout << numberOfArmies<<" "<<sourceTerritory->getArmyNum() << endl;

    //added for phase observer
    string advanceMsg = "";

    if (targetTerritory->getpId() == sourceTerritory->getpId()) {
        //If the source and target territory both belong to the player
        //Move the armies to the terr
        targetTerritory->addArmy(numberOfArmies);


        //added
        advanceMsg = "The target and defend territory both belong to Player " + to_string(player->name) + "\n";
    }
    else {
        // check if the two involved players are in negotiate
        if (targetTerritory->getpId() != 0 && player->IfNegotiate && targetPlayer->IfNegotiate)
        {
            // if true, this order would stop executing 
            sourceTerritory->addArmy(numberOfArmies);

            advanceMsg = "Player " + to_string(player->name) + " and Player" + to_string(targetPlayer->name)
                + " are in negotiate. No fight is initiated.\n";
            return advanceMsg;
        }

        if (targetTerritory->getpId() == 0) {
            //cout << "The target Player is Neutral player" << endl;
            //advanceMsg = "The target Player is Neutral player\n";
        }

        //start the war: find the number of armies that terr has smaller amrypool
        int numberOfRound;
        if (numberOfArmies <= targetTerritory->getArmyNum()) {
            numberOfRound = numberOfArmies;
        }
        else {
            numberOfRound = targetTerritory->getArmyNum();
        }
        //loop numberOfRound tims to start the fight
        for (int i = 0; i < numberOfRound; i++) {
            //Each attacking army unit involved has 60 % chances of killing one defending army.At the same time,
            //each defending army unit has 70 % chances of killing one attacking army unit.
            int attacker = rand() % 10;
            int defender = rand() % 10;
            if (attacker < 6) {
                //attacker will kill a defender and this round end 
                targetTerritory->setArmy(targetTerritory->getArmyNum()-1);
                /*cout << " Attacker from Player [" << sourceTerritory->getpId() <<
                    "] [" << sourceTerritory->gettName()
                    << "] kill a defender in Player [" << targetTerritory->getpId() <<
                    "[" << targetTerritory->gettName() << "]" << endl;*/

                advanceMsg += "Attacker from Player [" + to_string(sourceTerritory->getpId()) +"] [" 
                    + sourceTerritory->gettName() + "] kill a defender in Player [" + to_string(targetTerritory->getpId()) 
                    +"] [" + targetTerritory->gettName() + "]\n";
            }
            if (defender < 7) {
                //defender will kill a attacker
                numberOfArmies -= 1;
                //cout << " Defender from Player [" << targetTerritory->getpId() <<
                //    "] [" << targetTerritory->gettName()
                //    << "] kill an attacker in Player [" << sourceTerritory->getpId() <<
                //    "[" << sourceTerritory->gettName() << "]" << endl;

                advanceMsg += "Defender from Player [" + to_string(targetTerritory->getpId()) + "] [" 
                    + targetTerritory->gettName() + "] kill an attacker in Player [" + to_string(sourceTerritory->getpId())
                    + "] [" + sourceTerritory->gettName() + "]\n";
            }
        }
        //ending the fights and check the result
        // conqure happens
        if (targetTerritory->getArmyNum() == 0 && numberOfArmies != 0) {
            // only remove territory from actual player, not netural player
            if (targetTerritory->getpId() != 0) {
                targetPlayer->removeTerrByTID(targetTerritory->gettId());
            }
            targetTerritory->setpId(player->name);
            player->addTerr(targetTerritory);
            targetTerritory->addArmy(numberOfArmies);
            player->conquer = true;//player win the war and conquered a terr
            //cout << " Attacker from Player [" << sourceTerritory->getpId() <<
            //    "] [" << sourceTerritory->gettName()
            //    << "] won and conquered Player [" << targetTerritory->getpId() <<
            //    "] [" << targetTerritory->gettName() << "]" << endl;
            
            advanceMsg += "Attacker from Player [" + to_string(sourceTerritory->getpId()) + "] [" 
                + sourceTerritory->gettName() + "] won and conquered Player [" + to_string(targetTerritory->getpId())
                + "] [" + targetTerritory->gettName() + "]\n";
        }
        else
        {
            // send the survivors back to source territory
            sourceTerritory->addArmy(numberOfArmies);
            //cout << "conquer was not successful," << numberOfArmies <<
            //    " survivor went back to [" << sourceTerritory->gettName() << endl;
            advanceMsg += "conquer was not successful," + to_string(numberOfArmies)
                + " survivor went back to [" + sourceTerritory->gettName() + "]\n";
        }
        //cout << *this;//print a message to show detailes 
    }
    return advanceMsg;
}

const int Advance::getPriority() const
{
    return orderPriority;
}

void Advance::PrintMsg() const {
    cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Advance& toOutput) {
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Advance Order*" << " move " << toOutput.numberOfArmies
            << " army to territory[" << toOutput.targetTerritory->getpId() << "] ["
            << toOutput.targetTerritory->gettName() << "]" << endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Advance* Player[" << toOutput.player->name << "] has moved " 
                << toOutput.numberOfArmies<< " army to territory[" 
                << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "] . Army# ["
                << toOutput.targetTerritory->getArmyNum() << "] in ["
                << toOutput.targetTerritory->gettName() << "] " << endl;
        }
        else
        {
            out << "*Advance* order of player [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

//------------------------------Bomb----------------------------------------------
Bomb::Bomb() : Order(), sourceTerritory() {}

Bomb::Bomb(Player* player, Territory* sourceTerritory,
    Territory* targetTerritory)
    : Order(player, targetTerritory) {
    this->sourceTerritory = sourceTerritory;
}

Bomb::Bomb(const Bomb& toCopy) : Order(toCopy) {
    this->sourceTerritory = toCopy.sourceTerritory;
}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& rightSide) {
    Order::operator=(rightSide);
    sourceTerritory = rightSide.sourceTerritory;
    return *this;
}

bool Bomb::validate() {

    return (Order::validate() && !player->hasTerritory(targetTerritory->gettId()));
}

string Bomb::execute() {
    if (!validate()) {
        //cout << *this;
        return "";
    }
    wasExecuted = true;
    //half of the armies are removed from targer territory. 
    targetTerritory->setArmy(targetTerritory->getArmyNum() / 2);
    //cout << *this;
    return "";
}

void Bomb::PrintMsg() const {
    cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Bomb Order*" << " remove half armies in player [" 
            << toOutput.targetTerritory->getpId() << "] ["
            << toOutput.targetTerritory->gettName() << "]"<< endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Bomb* [" << toOutput.player->name << "] has removed half armies in player ["
                << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "]. Number of armies in target territory changes to "
                << toOutput.targetTerritory->getArmyNum() << endl;
        }
        else
        {
            out << "*Bomb* order of player [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

const int Bomb::getPriority() const
{
    return orderPriority;
}

//------------------------------------Blockade--------------------------------------------
Blockade::Blockade() : Order() {}

Blockade::Blockade(Player* player, Territory* targetTerritory)
    : Order(player, targetTerritory) {}

Blockade::Blockade(const Blockade& toCopy) : Order(toCopy) {}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& rightSide) {
    Order::operator=(rightSide);
    return *this;
}

const int Blockade::getPriority() const
{
    return orderPriority;
}

bool Blockade::validate() {
    //only terr player own can use blockade
    return (Order::validate() && player->hasTerritory(targetTerritory->gettId()));
}
string Blockade::execute() {
    if (!validate()) {
        cout << *this;
        return "";
    }
    wasExecuted = true;
    //double the armies in the target terr
    targetTerritory->setArmy(targetTerritory->getArmyNum() * 2);
    //rome this terr from player's own terr list
    //TODO:the player's toAtk list will add this target terr
    player->removeTerrByTID(targetTerritory->gettId());
    // ownership of the territory is transferred to the Neutral player
    //player->addToAttack(targetTerritory->gettId());
    targetTerritory->setpId(0);

    //cout << *this;

    return "";
}

void Blockade::PrintMsg() const {
    cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Blockade& toOutput) {
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Blockade Order*" << " double armies in territory ["
            << toOutput.targetTerritory->gettName() << "]" << endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Blockade* [" << toOutput.player->name << "] has double armies in player ["
                << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "] Number of armies in target territory changes to "
                << toOutput.targetTerritory->getArmyNum() << endl;
        }
        else
        {
            out << "*Blockade* order of player [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

//-----------------------------------------Negotiate------------------------------------------------
Negotiate::Negotiate() : Order(), targetPlayer(nullptr) { }

Negotiate::Negotiate(Player* player, Player* targetPlayer, Territory* targetTerritory)
    : Order(player, targetTerritory) {
    this->targetPlayer = targetPlayer;
}

Negotiate::Negotiate(const Negotiate& toCopy) : Order(toCopy) {
    targetPlayer = toCopy.targetPlayer;
}

Negotiate::~Negotiate() {

}

Negotiate& Negotiate::operator=(const Negotiate& rightSide) {
    Order::operator=(rightSide);
    targetPlayer = rightSide.targetPlayer;
    return *this;
}

const int Negotiate::getPriority() const
{
    return orderPriority;
}

bool Negotiate::validate() {
    // Check that the players negotiating aren't the same
    return (Order::validate() && player != targetPlayer);;
}

string Negotiate::execute() {
    if (!validate()) {
        return "";
    }
    wasExecuted = true;
    //the target player canot order advance and airlift order
    targetPlayer->IfNegotiate = true;
    player->IfNegotiate = true;
    //cout << *this;
    return "";
}

std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Negotiate Order* block attack from player ["
            << toOutput.targetPlayer->name << "] " << endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Negotiate* [" << toOutput.player->name << "] has block attack from player ["
                << toOutput.targetPlayer->name << "] " << endl;
        }
        else
        {
            out << "*Negotiate* order of player [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

//--------------------------------Airlift-----------------------------------------

Airlift::Airlift() : Order(), numberOfArmies(0), sourceTerritory(nullptr), targetPlayer(nullptr) {}

Airlift::Airlift(Player* player, int advanceArmies, Player* targetPlayer, Territory* sourceTerritory,
    Territory* targetTerritory)
    : Order(player, targetTerritory), numberOfArmies(advanceArmies) {
    this->sourceTerritory = sourceTerritory;
    this->targetPlayer = targetPlayer;
}

Airlift::Airlift(const Airlift& toCopy) : Order(toCopy) {
    numberOfArmies = toCopy.numberOfArmies;
    sourceTerritory = toCopy.sourceTerritory;
    targetPlayer = toCopy.targetPlayer;
}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& rightSide) {
    Order::operator=(rightSide);
    numberOfArmies = rightSide.numberOfArmies;
    sourceTerritory = rightSide.sourceTerritory;
    targetPlayer = rightSide.targetPlayer;
    return *this;
}

bool Airlift::validate() {
    bool isTargetValid = false;
    if (targetTerritory->getpId() == 0)
    {
        isTargetValid = true;
    }
    else if (targetPlayer == nullptr)
    {
        isTargetValid = false;
    }
    else
    {
        isTargetValid = targetPlayer->name == targetTerritory->getpId();
    }

    return (Order::validate()
        && sourceTerritory->getArmyNum() != 0
        && sourceTerritory->getpId() != 0
        && numberOfArmies != 0
        && sourceTerritory->getpId() == player->name
        && isTargetValid);
}

string Airlift::execute() {
    if (!validate()) {
        //cout << *this;
        return "";
    }
    wasExecuted = true;
    if (sourceTerritory->getArmyNum() < numberOfArmies)
    {
        numberOfArmies = sourceTerritory->getArmyNum();
    }
    //player will remove number of army from source terr
    sourceTerritory->setArmy(sourceTerritory->getArmyNum() - numberOfArmies);

    //added for phase observer
    string airliftMsg = "";

    if (targetTerritory->getpId() == sourceTerritory->getpId()) {
        //If the source and target territory both belong to the player
        //Move the armies to the terr
        targetTerritory->addArmy(numberOfArmies);
        //cout << "The target and defend territory both belong to Player" << player->name << endl;
        //cout << *this;

        //added
        airliftMsg = "The target and defend territory both belong to Player " + to_string(player->name) + "\n";
    }
    else {

        // check if the two involved players are in negotiate
        if (targetTerritory->getpId() != 0 && player->IfNegotiate && targetPlayer->IfNegotiate)
        {
            // if true, this order would stop executing 
            sourceTerritory->addArmy(numberOfArmies);
            //cout << "Player" << player->name << " and Player" << targetPlayer->name
            //    << " are in negotiate. No fight is initiated." << endl;

            airliftMsg = "Player " + to_string(player->name) + " and Player" + to_string(targetPlayer->name)
                + " are in negotiate. No fight is initiated.\n";
            return airliftMsg;
        }

        if (targetTerritory->getpId() == 0) {
            //cout << "The target Player is Neutral player" << endl;
            //airliftMsg = "The target Player is Neutral player\n";
        }
        //start the war: find the number of armies that terr has smaller amrypool
        int numberOfRound;
        if (numberOfArmies <= targetTerritory->getArmyNum()) {
            numberOfRound = numberOfArmies;
        }
        else {
            numberOfRound = targetTerritory->getArmyNum();
        }
        //loop numberOfRound tims to start the fight
        for (int i = 0; i < numberOfRound; i++) {
            //Each attacking army unit involved has 60 % chances of killing one defending army.At the same time,
            //each defending army unit has 70 % chances of killing one attacking army unit.
            int attacker = rand() % 10;
            int defender = rand() % 10;
            if (attacker < 6) {
                //attacker will kill a defender and this round end 
                targetTerritory->setArmy(targetTerritory->getArmyNum() - 1);
                /*cout << " Attacker from Player [" << sourceTerritory->getpId() <<
                    "] [" << sourceTerritory->gettName()
                    << "] kill a defender in Player [" << targetTerritory->getpId() <<
                    "[" << targetTerritory->gettName() << "]" << endl;*/

                airliftMsg += "Attacker from Player [" + to_string(sourceTerritory->getpId()) + "] ["
                    + sourceTerritory->gettName() + "] kill a defender in Player [" + to_string(targetTerritory->getpId())
                    + "] [" + targetTerritory->gettName() + "]\n";
            }
            if (defender < 7) {
                //defender will kill a attacker
                numberOfArmies -= 1;
                /*cout << " Defender from Player [" << targetTerritory->getpId() <<
                    "] [" << targetTerritory->gettName()
                    << "] kill a attacker in Player [" << sourceTerritory->getpId() <<
                    "[" << sourceTerritory->gettName() << "]" << endl;*/

                airliftMsg += "Defender from Player [" + to_string(targetTerritory->getpId()) + "] ["
                    + targetTerritory->gettName() + "] kill an attacker in Player [" + to_string(sourceTerritory->getpId())
                    + "] [" + sourceTerritory->gettName() + "]\n";
            }
        }
        //ending the fights and check the result
        // conqure happens
        if (targetTerritory->getArmyNum() == 0 && numberOfArmies != 0) {
            // only remove territory from actual player, not netural player
            if (targetTerritory->getpId() != 0) {
                targetPlayer->removeTerrByTID(targetTerritory->gettId());
            }
            targetTerritory->setpId(player->name);
            player->addTerr(targetTerritory);
            targetTerritory->addArmy(numberOfArmies);
            player->conquer = true;//player win the war and conquered a terr
            //cout << " Attacker from Player [" << sourceTerritory->getpId() <<
            //    "] [" << sourceTerritory->gettName()
            //    << "] won and conquered Player [" << targetTerritory->getpId() <<
            //    "[" << targetTerritory->gettName() << "]" << endl;

            airliftMsg += "Attacker from Player [" + to_string(sourceTerritory->getpId()) + "] ["
                + sourceTerritory->gettName() + "] won and conquered Player [" + to_string(targetTerritory->getpId())
                + "] [" + targetTerritory->gettName() + "]\n";
        }
        else
        {
            // send the survivors back to source territory
            sourceTerritory->addArmy(numberOfArmies);
            //cout << "conquer was not successful," << numberOfArmies <<
            //    " survivor went back to [" << sourceTerritory->gettName() << endl;

            airliftMsg += "conquer was not successful," + to_string(numberOfArmies)
                + " survivor went back to [" + sourceTerritory->gettName() + "]\n";

        }
        //cout << *this;//print a message to show detailes 

    }
    return airliftMsg;
}

void Airlift::PrintMsg() const {
    cout << *this;
}

const int Airlift::getPriority() const
{
    return orderPriority;
}

std::ostream& operator<<(std::ostream& out, const Airlift& toOutput) {
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Airlift Order* move " << toOutput.numberOfArmies
            << " army to territory [" << toOutput.targetTerritory->getpId() << "] ["
            << toOutput.targetTerritory->gettName() << "]" <<endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Airlift* Player[" << toOutput.player->name << "] has moved " << toOutput.numberOfArmies
                << " army to territory[" << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "] . Army# ["
                << toOutput.sourceTerritory->getArmyNum() << "] in [ "
                << toOutput.sourceTerritory->gettName() << "] " << endl;
        }
        else
        {
            out << "*Airlift* order of player [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

//======================================================================
// Orders List
OrdersList::OrdersList() {  }

OrdersList::OrdersList(const OrdersList& toCopy) {
    // TODO implement copy constructor correctly
    
}

OrdersList::~OrdersList() {

    for (auto order : ordersList) {
        delete order;
        order = nullptr;
    }
    
    ordersList.clear();
}

vector<Order*>& OrdersList::getOrderList()
{
    return ordersList;
}

OrdersList& OrdersList::operator=(const OrdersList& rightSide) {
    ordersList = rightSide.ordersList;
    return *this;
}

std::ostream& operator<<(std::ostream& outs, const OrdersList& toOutput) {
    std::vector<Order*> ordersList = toOutput.ordersList;
    //outs << "Orders list containing :" << std::endl;
    for (std::vector<Order*>::iterator it = ordersList.begin();
        it != ordersList.end(); ++it) {
        //std::cout << *(*it) << std::endl;
        (*it)->PrintMsg();
    }
    return outs;
}

void OrdersList::queue(Order* order) { ordersList.push_back(order); }

void OrdersList::move(int orderPosition, int newOrderPosition) {
    if (orderPosition < 1 || orderPosition > (int)ordersList.size()) {
        return;
    }
    int orderIndex{ orderPosition - 1 };
    int newOrderIndex{ newOrderPosition - 1 };
    newOrderIndex = std::min(std::max(newOrderIndex, 0), (int)ordersList.size());

    Order* toMove = (ordersList)[orderIndex];
    ordersList.erase(ordersList.begin() + orderIndex);
    ordersList.insert(ordersList.begin() + newOrderIndex, toMove);
}


// Position is 1-indexed instead of 0-indexed
// to make it more intuitive for users
void OrdersList::remove(int position) {
    if (position < 1 || position > (int)ordersList.size()) {
        return;
    }
    // Remove 1 to account for position being 1-indexed
    ordersList.erase(ordersList.begin() + position - 1);
}

Order* OrdersList::pop()
{
    if (ordersList.size() > 0)
    {
        Order* o = ordersList.at(0);
        remove(1);
        return o;
    }
    return nullptr;
}
