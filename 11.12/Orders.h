#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Map.h"
#include "Player.h"

using namespace std;


class Player;
class Territory;
class Graph;

class Order {
public:
    Order();
    Order(Player* player);
    Order(const Order& toCopy);
    virtual ~Order();
    Order& operator=(const Order& rightSide);

    virtual bool validate() = 0;
    virtual void execute() = 0;

    friend std::ostream& operator<<(std::ostream& out, const Order& toOutput);

protected:
    Player* player;
    bool wasExecuted{ false };

private:
    virtual std::ostream& doPrint(std::ostream& out) const = 0;
};

// Deploy armies to one of player's territories
class Deploy : public Order {
public:
    Deploy();
    Deploy(Player* player, Territory* territory);
    Deploy(const Deploy& toCopy);
    ~Deploy();
    Deploy& operator=(const Deploy& rightSide);

    virtual bool validate();
    virtual void execute();

private:
    Territory* territoryToDeploy;
    virtual std::ostream& doPrint(std::ostream& out) const;
};

// Advance armies from source to target territories, attacking if needed
class Advance : public Order {
public:
    Advance();
    Advance(Player* player, Territory* sourceTerritory,
        Territory* targetTerritory);
    Advance(const Advance& toCopy);
    ~Advance();
    Advance& operator=(const Advance& rightSide);

    friend std::ostream& operator<<(std::ostream& out, const Advance& toOutput);

    virtual bool validate();
    virtual void execute();

private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    virtual std::ostream& doPrint(std::ostream& out) const;
};

// Bomb an opponent's adjacent territory
class Bomb : public Order {
public:
    Bomb();
    Bomb(Player* player, Territory* sourceTerritory, Territory* targetTerritory);
    Bomb(const Bomb& toCopy);
    ~Bomb();
    Bomb& operator=(const Bomb& rightSide);

    friend std::ostream& operator<<(std::ostream& out, const Bomb& toOutput);

    virtual bool validate();
    virtual void execute();

private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    virtual std::ostream& doPrint(std::ostream& out) const;
};

// Triple armies on one of player's territories, making it neutral
class Blockade : public Order {
public:
    Blockade();
    Blockade(Player* player, Territory* territoryToBlockade);
    Blockade(const Blockade& toCopy);
    ~Blockade();
    Blockade& operator=(const Blockade& rightSide);

    friend std::ostream& operator<<(std::ostream& out, const Blockade& toOutput);

    virtual bool validate();
    virtual void execute();

private:
    Territory* territoryToBlockade;
    virtual std::ostream& doPrint(std::ostream& out) const;
};

// Negotiate peace for a turn between player and opponent
class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(Player* player, Player* opponent);
    Negotiate(const Negotiate& toCopy);
    ~Negotiate();
    Negotiate& operator=(const Negotiate& rightSide);

    friend std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput);

    virtual bool validate();
    virtual void execute();

private:
    Player* opponent;
    virtual std::ostream& doPrint(std::ostream& out) const;
};

// Advance armies from a player's territory to any territory
class Airlift : public Order {
public:
    Airlift();
    Airlift(Player* player, Territory* sourceTerritory,
        Territory* targetTerritory);
    Airlift(const Airlift& toCopy);
    ~Airlift();
    Airlift& operator=(const Airlift& rightSide);

    friend std::ostream& operator<<(std::ostream& out, const Airlift& toOutput);

    virtual bool validate();
    virtual void execute();

private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    virtual std::ostream& doPrint(std::ostream& out) const;
};

class OrdersList {
public:
    OrdersList();
    OrdersList( OrdersList& toCopy);
    ~OrdersList();
    OrdersList& operator=(const OrdersList& rightSide);

    // Add an order to the end of the orders list
    void queue(Order* o);

    // Takes order at position orderPosition and moves it at newOrderPosition,
    // shifting other orders accordingly Position" is 1-indexed instead of
    // 0-indexed (more intuitive to the player)
    void move(int orderPosition, int newOrderPosition);

    // Removes the order at position (i.e. 1-indexed) in the list
    void remove(int position);

    friend std::ostream& operator<<(std::ostream& outs,
        const OrdersList& toOutput);
    int getListSize();

    vector<Order*> ordersList;
};