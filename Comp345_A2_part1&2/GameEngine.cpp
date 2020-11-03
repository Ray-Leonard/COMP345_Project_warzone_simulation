//
// Created by Nian Liu on 2020-10-19.
//

#include "GameEngine.h"

#include <random>


GameEngine::GameEngine()
{
    m = nullptr;
    numOfPlayers = 0;
    observerFlag = false;
}
GameEngine::GameEngine(Map &map, int players, bool observerSwich)
{
    *m = map;
    numOfPlayers = players;
    this->observerFlag = observerSwich;
}
GameEngine::GameEngine(const GameEngine& gameEngine)
{
    m = gameEngine.m;
    numOfPlayers = gameEngine.numOfPlayers;
    observerFlag = gameEngine.observerFlag;
}

Map* GameEngine::readMap()
{
    cout << "enter map name: ";
    string path = "";
    cin >> path;
    MapLoader* ml = new MapLoader(path);//path: /Users/nianliu/Desktop/Comp345/Comp335_Project/untitled/europe.map
    cout << "The name of map file is: " << *ml << endl;
    return m = ml->loadingMap();
}
bool GameEngine::getMapFlag() {return m->validate();}
void GameEngine::setNumOfPlayer()
{
    int np = 0;
    while(true){
        START:
        cout<<"How many player (2-5): ";
        cin>>np;
        if(np<2){
            cout<<"The minimum player number is 2!"<<endl;
            goto START;
        }
        else if(np>5){
            cout<<"The maximum player number is 5!"<<endl;
            goto START;
        }
        else{
            numOfPlayers = np;
            for(int i=0;i<np;i++){
                string playerName = to_string(i);
                pl.push_back(new Player(playerName));
            }
            break;
        }
    }
}
int GameEngine::getNumOfPlayer() {return numOfPlayers;}
void GameEngine::shufflePlayers()
{
    shuffle(pl.begin(),pl.end(), std::mt19937(std::random_device()()));
}
void GameEngine::printPlayerQ()
{
    for(int i=0;i<numOfPlayers;i++){
        cout<<pl[i]->getName();
    }
    cout<<endl;
}
void GameEngine::printPlayerArmy()
{
    for(int i=0;i<numOfPlayers;i++){
        cout<<"Player["<<pl[i]->getName()<<"] has army: "<<pl[i]->army<<" ,";
    }
    cout<<endl;
}
void GameEngine::setObserverSwitch()
{
    string obs = "";
    cout<<"Need observer (Yes/No)? ";
    cin>>obs;
    transform(obs.begin(),obs.end(),obs.begin(),::toupper);
    if(obs=="YES"||obs=="Y"||obs=="1"){
        cout<<"need observer"<<endl;
        observerFlag = true;
    }
    else{
        cout<<"no need observer"<<endl;
        observerFlag = false;
    }
}
bool GameEngine::getObserverSwitch() {return observerFlag;}
void GameEngine::startupPhase()
{
    switch(numOfPlayers){
        case 2:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->army = 40;
            }
            break;
        case 3:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->army = 35;
            }
            break;
        case 4:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->army = 30;
            }
            break;
        case 5:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->army = 25;
            }
            break;
        default:
            cout<<"Number of player is invalidation!!!"<<endl;
    }
    int allT = m->allNodes.size();
    for(int i=0;i<allT;i++){
        int playerId = stoi(pl[i%(pl.size())]->getName());
        m->allNodes[i]->setpId(playerId);
        pl[i%(pl.size())]->territories.push_back(m->allNodes[i]);
    }

}

