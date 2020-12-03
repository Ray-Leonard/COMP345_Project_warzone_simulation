#ifndef untitled_GAMEENGINE_H
#define untitled_GAMEENGINE_H
#include <string>
#include <iostream>
#include "Map.h"
#include "MapLoader.h"
#include "Player.h"
#include "Cards.h"
#include "Orders.h"
#include "GameObservers.h"
using namespace std;

class GameEngine: public Subject {
private:
    Map* m;
    int numOfPlayers;
    bool observerFlag;
    vector<Player*> pl;
    Deck* deck;
    PhaseObserver* phaseObserver;
    StatisticObserver* statisticObserver;

    /*-----------Helper functions for loadingPhase() and startupPhase()--------------------*/
    // allows the game engine to read a map from user input and gracefully reject invalid map files
    void readMap();
    // allows the user to input the number of players
    void setNumOfPlayer();
    // allows the user to choose whether to turn on observer
    void setObserverSwitch();
    // helps to determine the order in which players play
    void shufflePlayers();
    /*----------------------------------------------------------------------------------*/


    /*------------------Printers-----------------*/
    // shows in which order that players are playing
    void printPlayerSequence();
    // prints a snapshot of how many armies each player have in their army pool
    void printPlayerArmy();
    // count the percentage of the world controlled by each player
    string getPercentOfWorld() const;
    /*-------------------------------------------*/


    /*------------Helpers for mainGameLoop()----------------*/
    // Called by mainGameLoop() to check for winner and losers every loop
    bool gameCheck();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    /*------------------------------------------------------*/


public:
    // constructors
    GameEngine();
    GameEngine(Map& map, int players, bool observerSwitch, Deck* _deck);
    GameEngine(const GameEngine& gameEngine);

    // destructor
    ~GameEngine();


    /* 
      The loading phase does the following:
    *   1. Ask the user to input a map name
    *   2. Check map validity, if invalid, go back to step 1
    *   3. Ask the user to select the # of players
    *   4. Check the validity of # of players, if invalid, go back to step 3
    *   5. Create players based on the # of players, and assign each of them an empty hand of cards(being done in Player constructor)
    *   6. Create a Deck
    *   7. Ask the user to turn on/off observer
    *   8. (optional) print all the informations up to now
    */
    void loadingPhase();

    /*
      The startupPhase does the following:
        1. shuffle the player's playing order
        2. assign them with an initial army 
            *If 2 players, A=40
            *If 3 players, A=35
            *If 4 players, A=30
            *If 5 players, A=25
        3. randomly assign all territories to each player in round-robin fashion
        4. output important information for demo and testing purpose. 
    */
    void startupPhase();

    /**/
    void mainGameLoop();



};


#endif 
