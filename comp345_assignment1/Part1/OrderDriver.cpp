
#define ORDERS_H
#ifdef ORDERS_H
#include <string>
#include <iostream>
#include "Map.h"
#include "Orders.h"
#include "Player.h"
using namespace std;

int main() {
    // Create some players and territories to do the tests
    Player* player = new Player{"player1"};
    Player* opponent = new Player("player2");
    
    Territory* playerTerritory2 = new Territory{ "Canada",4 };
    Territory* playerTerritory = new Territory{ "American",5 };
    Territory* opponentTerritory = new Territory{ "Cuba",3 };

    // Test the orders
    cout << "Testing the orders:" << endl << endl;
    Order* deploy = new Deploy(player, playerTerritory);
    Order* advance = new Advance(player, playerTerritory, opponentTerritory);
    Order* bomb = new Bomb(player, playerTerritory, opponentTerritory);
    Order* blockade = new Blockade(player, playerTerritory2);
    Order* negotiate = new Negotiate(player, opponent);
    Order* airlift = new Airlift(player, playerTerritory, opponentTerritory);

    

    cout << "Deploy is valid: " << deploy->validate() << endl;
    deploy->execute(); 
    cout << endl;
    cout << "Advance is valid: " << advance->validate() << endl;
    advance->execute();
    cout << endl;
    cout << "Bomb is valid: " << bomb->validate() << endl;
    bomb->execute();
    cout << endl;
    cout << "Blockade is valid: " << blockade->validate() << endl;
    blockade->execute();
    cout << endl;
    cout << "Negotiate is valid: " << negotiate->validate() << endl;
    negotiate->execute();
    cout << endl;
    cout << "Airlift is valid: " << airlift->validate() <<  endl;
    airlift->execute();
    cout << endl;

    // Test the orders list
    cout << endl
        << "===========================" << std::endl
        << std::endl
        << "Test the orderslist" << std::endl;

    OrdersList* ordersList = new OrdersList();
    ordersList->queue(deploy);
    ordersList->queue(advance);
    ordersList->queue(bomb);
    ordersList->queue(blockade);
    ordersList->queue(negotiate);
    ordersList->queue(airlift);

    std::cout << *ordersList << std::endl;

    // Move orders within the list
    // Expected results:  Deploy, Negotiate, Advance, Blockade, Bomb, Airlift
    ordersList->move(1, 5);
    ordersList->move(2, 4);
    ordersList->move(3, 1);

    cout << *ordersList << endl;

    // Remove orders
    // Expected results:  Negotiate, Advance, Bomb, Deploy
    ordersList->remove(3);
    ordersList->remove(5);

   cout << *ordersList << endl;

    // Finish the tests, delete everything

    delete ordersList;
    delete player;
    delete opponent;
    delete playerTerritory;
    delete opponentTerritory;

    
    // eliminate danling pointer
    ordersList = nullptr;
    player = nullptr;
    opponent = nullptr;
    playerTerritory = nullptr;
    opponentTerritory = nullptr;

    return 0;
};
#endif