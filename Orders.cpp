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
    ;
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

void Deploy::execute() {
    if (!validate()) {
        // when this order cannot be executed, the army deployed should be returned to the player
        // but this should not really be happening because player only deploy to its own territory returned by toDefend()
        player->armyPool += numberOfArmies;
        return;
    }
    wasExecuted = true;
    // add army to the territory
    targetTerritory->addArmy(numberOfArmies);

    // output a message to console indicating the effect of this order
    //cout << *this;
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
            out << "*Deploy* Player [" << toOutput.player->name << "] has deployed " << toOutput.numberOfArmies
                << " army to [" << toOutput.targetTerritory->gettName() << "] now with army# " << toOutput.targetTerritory->getArmyNum() << endl;
        }
        else
        {
            out << "*Deploy* order of [" << toOutput.player->name << "] was not a valid order!" << endl;
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

    return (Order::validate()&& OneOfAdjTerrl);
}

void Advance::execute() {
    if (!validate()) {
        cout << *this;
        return;
    }
    wasExecuted = true;
    if (sourceTerritory->getArmyNum() < numberOfArmies)
    {
        numberOfArmies = sourceTerritory->getArmyNum();
    }
    //player will remove number of army from source terr
    sourceTerritory->setArmy(sourceTerritory->getArmyNum() - numberOfArmies);
    //cout << numberOfArmies<<" "<<sourceTerritory->getArmyNum() << endl;

    if (targetTerritory->getpId() == sourceTerritory->getpId() || player->name == targetPlayer->name){
        //If the source and target territory both belong to the player
        //Move the armies to the terr
        targetTerritory->addArmy(numberOfArmies);
   /*     cout << "The target and defend territory both belong to Player " << player->name << endl;
        cout << *this;*/
    }else{  
            //If not, find the number of armies that terr has smaller amrypool
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
            /*        cout << " attacker from ["<< sourceTerritory->gettName()
                        <<"] kill a defender in ["<< targetTerritory->gettName()<<"]"<<endl;*/
                }
                if (defender < 7) {
                    //defender will kill a attacker
                    numberOfArmies -= 1;
                   /* cout << " defender in [" << targetTerritory->gettName()
                        << "] kill a attacker from [" << sourceTerritory->gettName() << "]" << endl;*/
                }
            }
            //ending the fights and check the result
            // conqure happens
            if (targetTerritory->getArmyNum() == 0 && numberOfArmies != 0) {
                targetTerritory->setpId(player->name);
                player->addTerr(targetTerritory);
                targetPlayer->removeTerrByTID(targetTerritory->gettId());
                targetTerritory->addArmy(numberOfArmies);
                player->conquer = true;//player win the war and conquered a terr
                cout << " attacker from [" << player->name 
                    << "] win and conquered [" << targetTerritory->gettName() << "]" << " of Player " << targetPlayer->name << endl;
            }
            else
            {
                // send the survivors back to source territory
                sourceTerritory->addArmy(numberOfArmies);
            }
            //cout << *this;//print a message to show detailes 

    }
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
            << " army to player[" << toOutput.targetTerritory->getpId() << "] ["
            << toOutput.targetTerritory->gettName() << "]" << endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Advance* Player[" << toOutput.player->name << "] has moved " << toOutput.numberOfArmies
                << " army to player[" << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "] . Army# ["
                << toOutput.sourceTerritory->getArmyNum() << "] in ["
                << toOutput.sourceTerritory->gettName() << "] " << endl;
        }
        else
        {
            out << "*Advance* order of [" << toOutput.player->name << "] was not a valid order!" << endl;
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

void Bomb::execute() {
    if (!validate()) {
        cout << *this;
        return;
    }
    wasExecuted = true;
    //half of the armies are removed from targer territory. 
    targetTerritory->setArmy(targetTerritory->getArmyNum() / 2);
    //cout << *this;
}

void Bomb::PrintMsg() const {
    cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
    //issue order phase
    if (!toOutput.wasExecuted && toOutput.player->currentPhase == 3) {
        out << "*Bomb Order*" << " remave half armies in player [" 
            << toOutput.targetTerritory->getpId() << "] ["
            << toOutput.targetTerritory->gettName() << "]"<< endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Bomb* [" << toOutput.player->name << "] has remaved half armies in player ["
                << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "]. Number of armies in target territory changes to "
                << toOutput.targetTerritory->getArmyNum() << endl;
        }
        else
        {
            out << "*Bomb* order of [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

const int Bomb::getPriority() const
{
    return orderPriority;
}

//------------------------------------Blockade--------------------------------------------
Blockade::Blockade() : Order() { //targetPlayer=nullptr; 
}

Blockade::Blockade(Player* player, Territory* targetTerritory)
    : Order(player, targetTerritory) {
    //this->targetPlayer = targetPlayer;
}

Blockade::Blockade(const Blockade& toCopy) : Order(toCopy) {
    //this->targetPlayer = toCopy.targetPlayer;
}

Blockade::~Blockade() {
    //delete targetPlayer;
}

Blockade& Blockade::operator=(const Blockade& rightSide) {
    Order::operator=(rightSide);
    //targetPlayer = rightSide.targetPlayer;
    return *this;
}

const int Blockade::getPriority() const
{
    return orderPriority;
}

bool Blockade::validate() {
    return (Order::validate() && player->hasTerritory(targetTerritory->gettId()));;
}
void Blockade::execute() {
    if (!validate()) {
        cout <<*this;
        return;
    }
    wasExecuted = true;
    //double the armies in the target terr
    targetTerritory->setArmy(targetTerritory->getArmyNum() * 2);
    //rome this terr from player's own terr list
    player->removeTerrByTID(targetTerritory->gettId());
    // ownership of the territory is transferred to the Neutral player
    targetTerritory->setpId(0);
    cout << *this;
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
                << toOutput.targetTerritory->gettName() << "]"<< endl;
        }
        else
        {
            out << "*Blockade* order of [" << toOutput.player->name << "] was not a valid order!" << endl;
        }
    }
    return out;
}

/*
Negotiate::Negotiate() : Order(), opponent() {}

Negotiate::Negotiate(Player* player, Player* opponent) : Order(player) {
    this->opponent = opponent;
}

Negotiate::Negotiate(const Negotiate& toCopy) : Order(toCopy) {
    this->opponent = toCopy.opponent;
}

Negotiate::~Negotiate() {
  
}

Negotiate& Negotiate::operator=(const Negotiate& rightSide) {
    Order::operator=(rightSide);
    opponent = rightSide.opponent;
    return *this;
}

bool Negotiate::validate() {
    // Check that the players negotiating aren't the same
    cout << "Order is validated." << endl;
    return true;
}

void Negotiate::execute() {
    if (!validate()) {
        return;
    }
    wasExecuted = true;
    cout << "A peace deal was striken.\n";
}

std::ostream& Negotiate::doPrint(std::ostream& out) const {
    out << "Negotiate order.";
    if (wasExecuted) {
        out << " This order was executed, its effect was [negotiate with other players].";
    }
    return out;
}

void Negotiate::PrintMsg() const {
    cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
    return toOutput.doPrint(out);
}
*/

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
    // Check if source and target territories are neighbors
    //a list having all adjacent territories belong to the source territory
    vector<int> adjTerr = sourceTerritory->getAdjacentTerritoryVec();
    bool OneOfAdjTerrl = false;

    for (unsigned int i = 0; i < adjTerr.size(); ++i)
    {
        if (targetTerritory->gettId() == adjTerr.at(i))
        {
            OneOfAdjTerrl = false;
        }
        else {
            OneOfAdjTerrl = true;
            break;
        }
    }

    return (Order::validate() && OneOfAdjTerrl);
}

void Airlift::execute() {
    if (!validate()) {
        cout << *this;
        return;
    }
    wasExecuted = true;
    if (sourceTerritory->getArmyNum() < numberOfArmies)
    {
        numberOfArmies = sourceTerritory->getArmyNum();
    }
    //player will remove number of army from source terr
    sourceTerritory->setArmy(sourceTerritory->getArmyNum() - numberOfArmies);
    //cout << numberOfArmies<<" "<<sourceTerritory->getArmyNum() << endl;

    if (targetTerritory->getpId() == sourceTerritory->getpId() || player->name == targetPlayer->name) {
        //If the source and target territory both belong to the player
        //Move the armies to the terr
        targetTerritory->addArmy(numberOfArmies);
        /*     cout << "The target and defend territory both belong to Player " << player->name << endl;
             cout << *this;*/
    }
    else {
        //If not, find the number of armies that terr has smaller amrypool
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
                /*        cout << " attacker from ["<< sourceTerritory->gettName()
                            <<"] kill a defender in ["<< targetTerritory->gettName()<<"]"<<endl;*/
            }
            if (defender < 7) {
                //defender will kill a attacker
                numberOfArmies -= 1;
                /* cout << " defender in [" << targetTerritory->gettName()
                     << "] kill a attacker from [" << sourceTerritory->gettName() << "]" << endl;*/
            }
        }
        //ending the fights and check the result
        // conqure happens
        if (targetTerritory->getArmyNum() == 0 && numberOfArmies != 0) {
            targetTerritory->setpId(player->name);
            player->addTerr(targetTerritory);
            targetPlayer->removeTerrByTID(targetTerritory->gettId());
            targetTerritory->addArmy(numberOfArmies);
            player->conquer = true;//player win the war and conquered a terr
            cout << " attacker from [" << player->name
                << "] win and conquered [" << targetTerritory->gettName() << "]" << " of Player " << targetPlayer->name << endl;
        }
        else
        {
            // send the survivors back to source territory
            sourceTerritory->addArmy(numberOfArmies);
        }
        //cout << *this;//print a message to show detailes 

    }
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
        out << "*Airlift Order*" << toOutput.player->name << "] has moved " << toOutput.numberOfArmies
            << " army to player[" << toOutput.targetTerritory->getpId() << "] ["
            << toOutput.targetTerritory->gettName() << "]" <<endl;
    }

    //execute phase output
    if (toOutput.player->currentPhase == 4) {
        if (toOutput.wasExecuted)
        {
            out << "*Airlift* Player[" << toOutput.player->name << "] has moved " << toOutput.numberOfArmies
                << " army to player[" << toOutput.targetTerritory->getpId() << "] ["
                << toOutput.targetTerritory->gettName() << "] . Army# ["
                << toOutput.sourceTerritory->getArmyNum() << "] in [ "
                << toOutput.sourceTerritory->gettName() << "] " << endl;
        }
        else
        {
            out << "*Airlift* order of [" << toOutput.player->name << "] was not a valid order!" << endl;
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
