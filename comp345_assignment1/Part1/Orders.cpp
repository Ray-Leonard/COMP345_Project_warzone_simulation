#include <algorithm>
#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include <vector>
#include<iostream>
using namespace std;

//============================================================
// Orders
Order::Order() : player() {}

Order::Order(Player* player) { this->player = player; }

Order::Order(const Order& toCopy) {
    player = toCopy.player;
    wasExecuted = toCopy.wasExecuted;
}

Order::~Order() {}

Order& Order::operator=(const Order& rightSide) {
    player = rightSide.player;
    wasExecuted = rightSide.wasExecuted;
    return *this;
}


std::ostream& operator<<(std::ostream& out, const Order& toOutput) {
    return toOutput.doPrint(out);
}

Deploy::Deploy() : Order(), territoryToDeploy() {}

Deploy::Deploy(Player* player, Territory* territory) : Order(player) {
    this->player = player;
    this->territoryToDeploy = territory;
}

Deploy::Deploy(const Deploy& toCopy) : Order(toCopy) {
    this->territoryToDeploy = toCopy.territoryToDeploy;
}

Deploy& Deploy::operator=(const Deploy& rightSide) {
    Order::operator=(rightSide);
    territoryToDeploy = rightSide.territoryToDeploy;
    return *this;
}

Deploy::~Deploy() {}

bool Deploy::validate() 
{
    cout << "Order is validated." << endl;
    return true;
 }

void Deploy::execute() {
    if (!validate()) {
        return;
    }
    wasExecuted = true;
    cout << "Deploying armies into some territory.\n";
}

std::ostream& Deploy::doPrint(std::ostream& out) const {
    out << "Deploy order.";
    if (wasExecuted) {
        out << " This order was executed, its effect was [deploy enemy].";
    }
    return out;
}

Advance::Advance() : Order(), sourceTerritory(), targetTerritory() {}

Advance::Advance(Player* player, Territory* sourceTerritory,
    Territory* targetTerritory)
    : Order(player) {
    this->sourceTerritory = sourceTerritory;
    this->targetTerritory = targetTerritory;
}
Advance::Advance(const Advance& toCopy) : Order(toCopy) {
    this->sourceTerritory = toCopy.sourceTerritory;
    this->targetTerritory = toCopy.targetTerritory;
}

Advance::~Advance() {}

Advance& Advance::operator=(const Advance& rightSide) {
    Order::operator=(rightSide);
    sourceTerritory = rightSide.sourceTerritory;
    targetTerritory = rightSide.targetTerritory;
    return *this;
}

bool Advance::validate() {
    // Check if source and target territories are neighbors
    cout << "Order is validated." << endl;
    return true;
}

void Advance::execute() {
    if (!validate()) {
        return;
    }
    wasExecuted = true;
    cout << "Advancing into another territory.\n";
}

std::ostream& Advance::doPrint(std::ostream& out) const {
    out << "Advance order.";
    if (wasExecuted) {
        out << " This order was executed, its effect was [advance army to another territory ].";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Advance& toOutput) {
    return toOutput.doPrint(out);
}

Bomb::Bomb() : Order(), sourceTerritory(), targetTerritory() {}

Bomb::Bomb(Player* player, Territory* sourceTerritory,
    Territory* targetTerritory)
    : Order(player) {
    this->sourceTerritory = sourceTerritory;
    this->targetTerritory = targetTerritory;
}

Bomb::Bomb(const Bomb& toCopy) : Order(toCopy) {
    this->sourceTerritory = toCopy.sourceTerritory;
    this->targetTerritory = toCopy.targetTerritory;
}

Bomb::~Bomb() {}

Bomb& Bomb::operator=(const Bomb& rightSide) {
    Order::operator=(rightSide);
    sourceTerritory = rightSide.sourceTerritory;
    targetTerritory = rightSide.targetTerritory;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Bomb& toOutput) {
    return toOutput.doPrint(out);
}

bool Bomb::validate() {
    cout << "Order is validated." << endl;
    return true;
}

void Bomb::execute() {
    if (!validate()) {
        return;
    }
    wasExecuted = true;
    cout << "Bombing an adjacent territory.\n";
}

std::ostream& Bomb::doPrint(std::ostream& out) const {
    out << "Bomb order.";
    if (wasExecuted) {
        out << " This order was executed, its effect was [Bombing an adjacent territory].";
    }
    return out;
}

Blockade::Blockade() : Order(), territoryToBlockade() {}

Blockade::Blockade(Player* player, Territory* territoryToBlockade)
    : Order(player) {
    this->territoryToBlockade = territoryToBlockade;
}

Blockade::Blockade(const Blockade& toCopy) : Order(toCopy) {
    this->territoryToBlockade = toCopy.territoryToBlockade;
}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& rightSide) {
    Order::operator=(rightSide);
    territoryToBlockade = rightSide.territoryToBlockade;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Blockade& toOutput) {
    return toOutput.doPrint(out);
}

bool Blockade::validate() {
    
    cout << "Order is validated." << endl;
    return true;
}

void Blockade::execute() {
    if (!validate()) {
        return;
    }
    wasExecuted = true;
    cout << "Blockading a territory.\n";
}

std::ostream& Blockade::doPrint(std::ostream& out) const {
    out << "Blockade order.";
    if (wasExecuted) {
        out << " This order was executed, its effect was [Blockad a territory].";
    }
    return out;
}

Negotiate::Negotiate() : Order(), opponent() {}

Negotiate::Negotiate(Player* player, Player* opponent) : Order(player) {
    this->opponent = opponent;
}

Negotiate::Negotiate(const Negotiate& toCopy) : Order(toCopy) {
    this->opponent = toCopy.opponent;
}

Negotiate::~Negotiate() {}

Negotiate& Negotiate::operator=(const Negotiate& rightSide) {
    Order::operator=(rightSide);
    opponent = rightSide.opponent;
    return *this;
}

bool Negotiate::validate() {
    // Check that the players negotiating aren't the same
    return player != opponent;
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

std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput) {
    return toOutput.doPrint(out);
}

Airlift::Airlift() : Order(), sourceTerritory(), targetTerritory() {}

Airlift::Airlift(Player* player, Territory* sourceTerritory,
    Territory* targetTerritory)
    : Order(player) {
    this->sourceTerritory = sourceTerritory;
    this->targetTerritory = targetTerritory;
}

Airlift::Airlift(const Airlift& toCopy) : Order(toCopy) {
    this->sourceTerritory = toCopy.sourceTerritory;
    this->targetTerritory = toCopy.targetTerritory;
}

Airlift::~Airlift() {}

Airlift& Airlift::operator=(const Airlift& rightSide) {
    Order::operator=(rightSide);
    sourceTerritory = rightSide.sourceTerritory;
    targetTerritory = rightSide.targetTerritory;
    return *this;
}

bool Airlift::validate() {
    cout << "Order is validated." << endl;
    return true;
}

void Airlift::execute() {
    if (!validate()) {
        return;
    }
    wasExecuted = true;
    cout << "Airlifting into another territory!\n";
}

std::ostream& Airlift::doPrint(std::ostream& out) const {
    out << "Airlift order.";
    if (wasExecuted) {
        out << " This order was executed, its effect was [airlifting into another territory].";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Airlift& toOutput) {
    return toOutput.doPrint(out);
}

//======================================================================
// Orders List
OrdersList::OrdersList() { ordersList = new std::vector<Order*>(); }

OrdersList::OrdersList(const OrdersList& toCopy) {
    // TODO implement copy constructor correctly
    ordersList = toCopy.ordersList;
}

OrdersList::~OrdersList() {

    for (auto order : *ordersList) {
        delete order;
    }
    ordersList->clear();
    delete ordersList;
    ordersList = nullptr;
}

OrdersList& OrdersList::operator=(const OrdersList& rightSide) {
    ordersList = rightSide.ordersList;
    return *this;
}

std::ostream& operator<<(std::ostream& outs, const OrdersList& toOutput) {
    std::vector<Order*>* ordersList = toOutput.ordersList;
    outs << "Orders list containing :" << std::endl;
    for (std::vector<Order*>::iterator it = ordersList->begin();
        it != ordersList->end(); ++it) {
        std::cout << *(*it) << std::endl;
    }
    return outs;
}

void OrdersList::queue(Order* order) { ordersList->push_back(order); }

void OrdersList::move(int orderPosition, int newOrderPosition) {
    if (orderPosition < 1 || orderPosition > ordersList->size()) {
        return;
    }
    int orderIndex{ orderPosition - 1 };
    int newOrderIndex{ newOrderPosition - 1 };
    newOrderIndex = std::min(std::max(newOrderIndex, 0), (int)ordersList->size());

    Order* toMove = (*ordersList)[orderIndex];
    ordersList->erase(ordersList->begin() + orderIndex);
    ordersList->insert(ordersList->begin() + newOrderIndex, toMove);
}

// Position is 1-indexed instead of 0-indexed
// to make it more intuitive for users
void OrdersList::remove(int position) {
    if (position < 1 || position > ordersList->size()) {
        return;
    }
    // Remove 1 to account for position being 1-indexed
    ordersList->erase(ordersList->begin() + position - 1);
}

