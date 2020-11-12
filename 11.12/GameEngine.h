//
// Created by Nian Liu on 2020-10-19.
//

#ifndef untitled_GAMEENGINE_H
#define untitled_GAMEENGINE_H
#include <string>
#include <iostream>
#include "Map.h"
#include "MapLoader.h"
#include "Player.h"
#include "Cards.h"
using namespace std;

class GameEngine {
private:
    Map* m;
    int numOfPlayers;
    bool observerFlag;
    vector<Player*> pl;
    Deck* deck;

    // allows the game engine to read a map from user input and gracefully reject invalid map files
    void readMap();
    // allows the user to set the number of territories
    void setNumOfPlayer();
    // allows the user to choose whether to turn on observer
    void setObserverSwitch();
    // helps to determine the order in which players play
    void shufflePlayers();
    // shows in which order that players are playing
    void printPlayerSequence();
    // prints a snapshot of how many armies each player have in their army pool
    void printPlayerArmy();
    // prints a snapshot of which territory each player has
    void printPlayerTerritory();


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
    void startupPhase();
    bool gameCheck();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    void mainGameLoop();
};


#endif 
