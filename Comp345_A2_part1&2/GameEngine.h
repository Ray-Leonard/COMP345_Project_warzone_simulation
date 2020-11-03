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
using namespace std;

class GameEngine {
public:
    Map* m;
    int numOfPlayers;
    bool observerFlag;
    vector<Player*> pl;


    GameEngine();
    GameEngine(Map &map,int players,bool observerSwitch);
    GameEngine(const GameEngine& gameEngine);


    Map* readMap();
    bool getMapFlag();
    void setNumOfPlayer();
    int getNumOfPlayer();
    void shufflePlayers();
    void printPlayerQ();
    void printPlayerArmy();
    void setObserverSwitch();
    bool getObserverSwitch();
    void startupPhase();

};


#endif //DEMO11_GAMEENGINE_H
