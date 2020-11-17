#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Map.h"


using namespace std;

class Player;

// pure virtual class(or abstract class)
class Order {
protected:
    Player* player;
    Territory* targetTerritory;
    bool wasExecuted;
    bool isEnabled;
public:
    Order();
    Order(Player* player, Territory* target);
    Order(const Order& toCopy);
    virtual ~Order();
    Order& operator=(const Order& rightSide);

    virtual bool validate();
    virtual string execute() = 0;
    virtual const int getPriority() const = 0;
    void disableOrder();
    virtual Player* getPlayer(); //added virtual
    virtual void PrintMsg() const; //added
    Territory* getTargetTerritory();

    friend ostream& operator<<(std::ostream& out, const Order& toOutput);
};


// Deploy armies to one of player's territories
class Deploy : public Order {
public:
    static const int orderPriority = 5;
    Deploy();
    Deploy(Player* player, Territory* territory, int requestedNumberOfArmies);
    Deploy(const Deploy& toCopy);
    ~Deploy();
    Deploy& operator=(const Deploy& rightSide);

    bool validate();
    string execute();
    const int getPriority() const;
    void PrintMsg() const; //added
    friend ostream& operator<<(std::ostream& out, const Deploy& toOutput);

private:
    int numberOfArmies;

};


// Advance armies from source to target territories, attacking if needed
class Advance : public Order {
public:
    static const int orderPriority = 1;
    Advance();
    Advance(Player* player, int advanceArmies, Player* targetPlayer, Territory* sourceTerritory,
        Territory* targetTerritory);
    Advance(const Advance& toCopy);
    ~Advance();
   
     Advance& operator=(const Advance& rightSide);
    virtual bool validate();
    virtual string execute();
    void PrintMsg() const; //added
    friend std::ostream& operator<<(std::ostream& out, const Advance& toOutput);
    const int getPriority() const;

private:
    int numberOfArmies;
    Territory* sourceTerritory;
    Player* targetPlayer;
   
};

// Bomb an opponent's adjacent territory
class Bomb : public Order {
public:
    static const int orderPriority = 0;
    Bomb();
    Bomb(Player* player, Territory* sourceTerritory, Territory* targetTerritory);
    Bomb(const Bomb& toCopy);
    ~Bomb();
    Bomb& operator=(const Bomb& rightSide);

    virtual bool validate();
    virtual string execute();
    void PrintMsg() const; //added
    const int getPriority() const;
    friend std::ostream& operator<<(std::ostream& out, const Bomb& toOutput);

private:
    Territory* sourceTerritory;
};

// Triple armies on one of player's territories, making it neutral
class Blockade : public Order {
public:
    static const int orderPriority = 3;
    Blockade();
    Blockade(Player* player, Territory* targetTerritory);
    Blockade(const Blockade& toCopy);
    ~Blockade();
    Blockade& operator=(const Blockade& rightSide);
    const int getPriority() const;
    void PrintMsg() const; //added
    friend std::ostream& operator<<(std::ostream& out, const Blockade& toOutput);

    virtual bool validate();
    virtual string execute();

private:
    //Player* targetPlayer;
};

// Negotiate peace for a turn between player and opponent
class Negotiate : public Order {
public:
    static const int orderPriority = 2;
    Negotiate();
    Negotiate(Player* player, Player* targetPlayer, Territory* targetTerritory);
    Negotiate(const Negotiate& toCopy);
    ~Negotiate();
    Negotiate& operator=(const Negotiate& rightSide);
    const int getPriority() const;
    friend std::ostream& operator<<(std::ostream& out, const Negotiate& toOutput);

    virtual bool validate();
    virtual string execute();

private:
    Player* targetPlayer;
};

// Airliftr tells a certain number of armies taken from a source territory to be moved to a target territory,
class Airlift : public Order {
public:
    static const int orderPriority = 4;
    Airlift();
    Airlift(Player* player, int advanceArmies, Player* targetPlayer, Territory* sourceTerritory,
        Territory* targetTerritory);
    Airlift(const Airlift& toCopy);
    ~Airlift();
    Airlift& operator=(const Airlift& rightSide);

    virtual bool validate();
    virtual string execute();
    const int getPriority() const;
    void PrintMsg() const; //added
    friend std::ostream& operator<<(std::ostream& out, const Airlift& toOutput);

private:
    int numberOfArmies;
    Territory* sourceTerritory;
    Player* targetPlayer;

};


class OrdersList {
public:
    OrdersList();
    OrdersList(const OrdersList& toCopy);
    ~OrdersList();
    OrdersList& operator=(const OrdersList& rightSide);

    // Add an order to the end of the orders list
    void queue(Order* o);

    // access the ordersList
    std::vector<Order*>& getOrderList();

    // Takes order at position orderPosition and moves it at newOrderPosition,
    // shifting other orders accordingly Position" is 1-indexed instead of
    // 0-indexed (more intuitive to the player)
    void move(int orderPosition, int newOrderPosition);

    // Removes the order at position (i.e. 1-indexed) in the list
    void remove(int position);

    // get the first order in the list
    Order* pop();

    friend std::ostream& operator<<(std::ostream& outs,
        const OrdersList& toOutput);

private:
    vector<Order*> ordersList;
};

// comparison structure for order priority
struct OrderByPriority
{
    bool operator() (Order* lhs, Order* rhs)
    {
        return lhs->getPriority() < rhs->getPriority();
    }
};