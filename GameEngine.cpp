//
// Created by Nian Liu on 2020-10-19.
//

#include "GameEngine.h"

#include <random>
#include <iostream>

// default constructor
GameEngine::GameEngine()
{
    m = nullptr;
    numOfPlayers = 0;
    observerFlag = false;
    deck = new Deck();
}

// argumented constructor
GameEngine::GameEngine(Map &map, int players, bool observerSwich, Deck* _deck)
{
    *m = map;
    numOfPlayers = players;
    this->observerFlag = observerSwich;
    deck = _deck;
}
// copy constructor
GameEngine::GameEngine(const GameEngine& gameEngine)
{
    m = gameEngine.m;
    numOfPlayers = gameEngine.numOfPlayers;
    observerFlag = gameEngine.observerFlag;
    // calls the copy constructor of Deck class
    deck = new Deck(*gameEngine.deck);
    // copy over the list of players
    for (unsigned int i = 0; i < gameEngine.pl.size(); ++i)
    {
        Player* temp_player = new Player(*pl.at(i));    // rely on the Player's copy constructor
        pl.push_back(temp_player);
    }
}

GameEngine::~GameEngine()
{
    delete m;
    m = nullptr;
    delete deck;
    deck = nullptr;
    for (int i = 0; i < numOfPlayers; ++i)
    {
        delete pl.at(i);
        pl.at(i) = nullptr;
    }
    pl.clear();
}


void GameEngine::readMap()
{
    bool validFile = true;
    do {
        validFile = true;
        cout << "enter map name: ";
        string path = "";
        cin >> path;

        MapLoader* ml = new MapLoader(path);
        try
        {
            m = ml->loadingMap();
        }
        catch (int e)
        {
            validFile = false;
            delete m;
            m = nullptr;
            delete ml;
            ml = nullptr;
        }
    } while (!validFile);
}


void GameEngine::setNumOfPlayer()
{
    int np = 0;
    while(true){
        cout<<"How many player (2-5): ";
        cin>>np;
        if(np<2){
            cout<<"The minimum player number is 2!"<<endl;
            continue;
        }
        else if(np>5){
            cout<<"The maximum player number is 5!"<<endl;
            continue;
        }
        else{
            numOfPlayers = np;
            for(int i=0;i<np;i++){
                string playerName = to_string(i+1);
                pl.push_back(new Player(playerName));
            }
            break;
        }
    }
}

void GameEngine::shufflePlayers()
{
    shuffle(pl.begin(),pl.end(), std::mt19937(std::random_device()()));
}

void GameEngine::printPlayerSequence()
{
    cout << "The player is now playing in the following order:" << endl;
    for(int i=0;i<numOfPlayers;i++){
        cout << "Player" << pl[i]->getName() << " ";
    }
    cout << endl << endl;
}

void GameEngine::printPlayerArmy()
{
    cout << "The players have the following army in startup phase: " << endl;
    for(int i = 0; i < numOfPlayers; ++i){
        cout << "Player[" << pl[i]->getName() << "] has army: " << pl[i]->army << endl;
    }
    cout << endl;
}

void GameEngine::printPlayerTerritory()
{
    for (int i = 0; i < numOfPlayers; i++) {
        cout << "Player[" << pl.at(i)->getName() << "] owns the following territories:" << endl;
        pl[i]->printTV();
    }
    cout << endl;
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
    cout << endl;
}

void GameEngine::loadingPhase()
{
    // loading phase part 1: select a correct map
    bool validMap = false;
    do {
        // 1. ask the user to input a map file and reject invalid files
        readMap();
        // 2. check validity of map and reject invalid maps
        validMap = m->validate();
        // delete the invalid map
        if (!validMap)
        {
            cout << "Please make sure the map is valid!" << endl;
            delete m;
            m = nullptr;
        }
    } while (!validMap);

    // loading phase part 2: set number of players
    setNumOfPlayer();

    // loading phase part 3: set observer
    setObserverSwitch();
}

void GameEngine::startupPhase()
{
    // shuffle the player's playing order
    shufflePlayers();
    
    // assign them with an initial army
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

    // output information for player
    printPlayerSequence();
    printPlayerArmy();

    // randomly assign territory to each player in round-robin fashion
    int territoryCount = m->allNodes.size();
    // make a vector of territory index and shuffle them to ensure a randomized result
    vector<int> territoryIndex;
    for (int i = 0; i < territoryCount; ++i)
    {
        territoryIndex.push_back(i);
    }
    shuffle(territoryIndex.begin(), territoryIndex.end(), std::mt19937(std::random_device()()));
    // begine to assign territories to players in the player list in round-robin fashion
    for (int i = 0; i < territoryCount; ++i)
    {
        pl.at(i % numOfPlayers)->territories.push_back(m->allNodes.at(territoryIndex.at(i)));
    }

    // output territories of players
    printPlayerTerritory();
}

int main()
{
    // create a game engine object
    GameEngine* ge = new GameEngine();
    // execute the loading phase
    ge->loadingPhase();
    // execute the startup phase
    ge->startupPhase();

    return 0;
}