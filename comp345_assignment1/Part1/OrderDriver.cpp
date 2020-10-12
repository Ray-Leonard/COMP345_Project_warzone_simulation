#define _CRTDBG_MAP_ALLOC
#define ORDERS_H
#ifdef ORDERS_H
#include <string>
#include <iostream>
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include <crtdbg.h>

using namespace std;

int main() {
    // Create some players and territories to do the tests
 /*   Player* player = new Player{"player1"};
    Player* opponent = new Player("player2");*/
    
    /*Territory* playerTerritory2 = new Territory{ "Canada",4 };
    Territory* playerTerritory = new Territory{ "American",5 };
    Territory* opponentTerritory = new Territory{ "Cuba",3 };*/

    // Test the orders
    cout << "Testing the orders:" << endl << endl;
    Order* deploy = new Deploy();
    Order* advance = new Advance();
    Order* bomb = new Bomb();
    Order* blockade = new Blockade();
    Order* negotiate = new Negotiate();
    Order* airlift = new Airlift();


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
        << "===========================" << endl
        << endl
        << "Test the orderslist" << endl;

    OrdersList* ordersList = new OrdersList();
    ordersList->queue(deploy);
    ordersList->queue(advance);
    ordersList->queue(bomb);
    ordersList->queue(blockade);
    ordersList->queue(negotiate);
    ordersList->queue(airlift);

    cout << *ordersList << endl;

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
    /*delete player;
    delete opponent;
    delete opponentTerritory;
    delete playerTerritory;
    delete playerTerritory2;*/
    /*delete deploy;*/
    /*delete airlift;*/
   /* delete advance;
    delete bomb;
    delete blockade;
    delete negotiate;
    */
    
    // eliminate danling pointer
    ordersList = nullptr;
    /*player = nullptr;
    opponent = nullptr;
    opponentTerritory = nullptr;
    playerTerritory = nullptr;
    playerTerritory2 = nullptr;*/
    deploy = nullptr;
    airlift = nullptr;
    advance = nullptr;
    bomb = nullptr;
    blockade = nullptr;
    negotiate = nullptr;

    _CrtDumpMemoryLeaks();
    return 0;
};
#endif
