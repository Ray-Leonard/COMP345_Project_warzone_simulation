#include "GameEngine.h"
#include "GameObservers.h"
#include <random>
#include <iostream>
#include <queue>
#include <thread>
#include <chrono>

// default constructor
GameEngine::GameEngine()
{
    m = nullptr;
    numOfPlayers = 0;
    observerFlag = false;
    deck = new Deck();
    deck->initialize();
    phaseObserver = nullptr;
    statisticObserver = nullptr;
}

// argumented constructor
GameEngine::GameEngine(Map &map, int players, bool observerSwich, Deck* _deck)
{
    *m = map;
    numOfPlayers = players;
    this->observerFlag = observerSwich;
    deck = _deck;
    phaseObserver = nullptr;
    statisticObserver = nullptr;
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
    phaseObserver = new PhaseObserver(*gameEngine.phaseObserver);
    statisticObserver = new StatisticObserver(*gameEngine.statisticObserver);
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
    delete phaseObserver;
    phaseObserver = nullptr;
    delete statisticObserver;
    statisticObserver = nullptr;
    pl.clear();
}


void GameEngine::readMap()
{
    bool validFile = true;
    do {
        validFile = true;
        Map::printAllMapNames();

        string mapType = "";
        cout << "Select map type (Initial/Conquest): ";
        cin >> mapType;

        cout << "enter map name: ";
        string path = "";
        cin >> path;

        transform(mapType.begin(), mapType.end(), mapType.begin(), ::toupper);

        MapLoader* ml = nullptr;
        ConquestFileReaderAdapter* cfm = nullptr;

        try
        {
            if (mapType == "INITIAL") {
                ml = new MapLoader(path);
                m = ml->loadingMap();
            }
            else {
                cfm = new ConquestFileReaderAdapter(new ConquestFileReader(path));
                m = cfm->loadingMap();
            }
        }
        catch (int e)
        {
            validFile = false;
            delete m;
            m = nullptr;
            delete ml;
            ml = nullptr;
            delete cfm;
            cfm = nullptr;
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
            for(int i = 0; i < np; i++){
                pl.push_back(new Player(i + 1));
                pl.at(i)->cards->add(deck->draw());
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
    
    string playerOrder = "";
    //cout << "The player is now playing in the following order:" << endl;
    Player* p = nullptr;
    for(int i = 0; i < numOfPlayers; i++){
        playerOrder = playerOrder.append(" Player " + to_string(pl[i]->name));
        p = (pl[i]);
        //cout << pl[i]->name << " ";
    }
    notify(nullptr, p, NULL, playerOrder); //delete p in PhaseObserver
    //cout << endl << endl;
}

void GameEngine::printPlayerArmy()
{
    //cout << "The players have the following army in startup phase: " << endl;
    for(int i = 0; i < numOfPlayers; ++i){
        pl.at(i)->currentPhase = 1;
        notify(nullptr, pl[i], NULL, "");
        //cout << "[" << pl[i]->name << "] has army: " << pl[i]->armyPool << endl;
    }
    //cout << endl;
}


string GameEngine::getPercentOfWorld() const {
    // get percentage of the world controlled by each player
    string percentMsg = "";
    for (int i = 0; i < numOfPlayers; ++i)
    {
        double perc = (double)pl.at(i)->territories.size() / m->getNumOfTrritories() * 100;

        //if the player is not eliminated
        if (pl.at(i)->name) {
            percentMsg += "Player [" + to_string(pl.at(i)->name) + "] owns " + to_string(perc) + "% of the world.\n";
        }
    }
    return percentMsg;
}

void GameEngine::setObserverSwitch()
{
    string phaseObs = "";
    string statisticObs = "";
    cout << "Need phase observer (Yes/No)? ";
    cin>> phaseObs;
    cout << "Need statistic observer (Yes/No)? ";
    cin >> statisticObs;
    transform(phaseObs.begin(), phaseObs.end(), phaseObs.begin(), ::toupper);
    transform(statisticObs.begin(), statisticObs.end(), statisticObs.begin(), ::toupper);

    if(phaseObs == "YES" || phaseObs == "Y" || phaseObs =="1"){
        cout << "Need phase observer" << endl;
        observerFlag = true;
        phaseObserver = new PhaseObserver(this);
        //statisticObserver = new StatisticObserver(this);
    }
    if (statisticObs == "YES" || statisticObs == "Y" || statisticObs == "1") {
        cout << "Need statistic observer" << endl;
        observerFlag = true;
        statisticObserver = new StatisticObserver(this);
    }
    if(!observerFlag)
    {
        cout<< "no need observer" << endl;
        //observerFlag = false;
    }
    cout << endl;
}

bool GameEngine::gameCheck()
{
    vector<int> loserName;
    //loop for all players to identify any losers
    for (int i = 0; i < numOfPlayers; ++i)
    {
        if (pl.at(i)->getTerrNumber() == 0)
        {
            loserName.push_back(pl.at(i)->name);
            notify(m, pl.at(i), NULL, "eliminate");
        }
    }

    // delete all losers
    for (unsigned int i = 0; i < loserName.size(); ++i)
    {
        for (unsigned int j = 0; j < pl.size(); ++j)
        {
            if (pl.at(j)->name == loserName.at(i))
            {
                delete pl.at(j);
                pl.at(j) = nullptr;
                pl.erase(pl.begin() + j);
                numOfPlayers--;
                break;
            }
        }
    }

    //the players owns all the territories in the map win
    //no one won the game the neutral plater own all terr
    if (pl.at(0)->getTerrNumber() == m->getNumOfTrritories()
        && pl.size() == 1)
    {
        return true;
    }
    return false;
}

void GameEngine::issueOrdersPhase()
{
    // call the issueOrder() on each player until they have no order to issue anymore
    for (unsigned int i = 0; i < pl.size(); ++i)
    {
        pl.at(i)->currentPhase = 3;
        while (pl.at(i)->issueOrder(m, deck, pl)) {}

        // after this player is done issuing orders, reset toAdvanceTime
        pl.at(i)->resetToAdvanceTime();

        //phase observer
        notify(nullptr, pl.at(i), NULL, "");
    }
}

void GameEngine::executeOrdersPhase()
{
    // create a big list of orders consisting of all player's orders in priority
    priority_queue<Order*, vector<Order*>, OrderByPriority> allOrders;
    for (unsigned int i = 0; i < pl.size(); ++i)
    {
        pl.at(i)->currentPhase = 4;
        // add all orders from this player to allOrders queue
        while (pl.at(i)->orders->getOrderList().size() > 0)
        {
            Order* o = pl.at(i)->orders->pop();
            //cout <<"A"<< pl.at(i)->name;
            allOrders.push(o);
        }
        
    }

    // then execute all the orders
    while (allOrders.size() > 0)
    {
        //output the order one player by one player
        //cout <<"B"<< pl.at(i)->name;
        Order* o = allOrders.top();
        string msg = o->execute();
        allOrders.pop();
        notify(nullptr, o->getPlayer(), o, msg); //phase observer
        delete o;
        o = nullptr;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // when finish executing all orders, check if any player needs to draw cards
    for (int i = 0; i < numOfPlayers; i++)
    {
        // check if conquer happend in this round 
        if (pl.at(i)->conquer)
        {
            // draw a card from the deck
            Card* c = deck->draw();
            // if player cannot have more cards
            if (!pl.at(i)->cards->add(c))
            {
                // put the card back into the deck
                deck->putBack(c);
            }
            pl.at(i)->conquer = false;
        }
        // reset IfNegotiate to false
        pl.at(i)->IfNegotiate = false;
    }
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
                pl[i]->armyPool = 40;
            }
            break;
        case 3:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->armyPool = 35;
            }
            break;
        case 4:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->armyPool = 30;
            }
            break;
        case 5:
            for(int i=0;i<numOfPlayers;i++){
                pl[i]->armyPool = 25;
            }
            break;
        default:
            cout<<"Number of player is invalidation!!!"<<endl;
    }

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

    // assign territories the players' ID 
    for (int i = 0; i < numOfPlayers; ++i)
    {
        pl.at(i)->setTerrPID();
        
    }
    

    // phase observer: output information for player
    printPlayerSequence();
    printPlayerArmy();


    string percentMsg = getPercentOfWorld();

    //statistic observer
    notify(m, NULL, NULL, percentMsg);
}

//Give the number of armiea to players 
void GameEngine::reinforcementPhase()
{
    for (int i = 0; i < numOfPlayers; i++) {
        int originalArmyPool = pl.at(i)->armyPool; //Yilu: original army # 

        //Players are given a number of armies that depends on the number of territories they own / 3 
        //each term player will get at lest 3 armies
        if (pl.at(i)->getTerrNumber() / 3 < 3) {
            pl.at(i)->armyPool += 3;
        }
        else {
            pl.at(i)->armyPool += pl.at(i)->getTerrNumber() / 3;
        }

        // To check for any bonus army
        // loop over all continents in the map
        for (int j = 0; j < m->getNumOfContinents(); ++j)
        {
            pl.at(i)->currentPhase = 2;
            bool isControlContinent = true;
            Continent* c = m->allContinents.at(j);
            // loop over all territories in this continent
            for (int k = 0; k < c->getNumOfTerritories(); ++k)
            {
                // check if a player owns all territories in this continent
                isControlContinent = isControlContinent && (pl.at(i)->hasTerritory(c->getAllTerritories().at(k)));
            }

            // when the player do control all territories in the continent, add bonus army to the player
            if (isControlContinent)
            {
                pl.at(i)->armyPool += c->getBounusArmyNum();
            }
        }

        //calculate army # that is added to each player
        int addedArmies = pl.at(i)->armyPool - originalArmyPool; 

        //call PhaseObserver
        notify(nullptr, pl.at(i), NULL, to_string(addedArmies));
    }

}

void GameEngine::mainGameLoop()
{
    //end the game if there is only one player
    int i = 0;
    //%only run 5 game turns
    while (i<10) {
        i += 1;
        //set player strategy 
        for (int i = 0; i < pl.size(); i++)
        {
            int RandInt = rand() % 4;
            // randInt=0,strateg = Neutral
            // randint=1,strategy = Human
            // randint=2,strategy = Aggressive
            // randint=3,strategy = Benevolent
            pl.at(i)->setStrategy(RandInt);
            //print out player strategy
            string strategy;
            switch (RandInt)
            {
            case 0:
                strategy = "Neutral player.";
                break;
            case 1:
                strategy = "Human player.";
                break;
            case 2:
                strategy = "Aggresive player.";
                break;
            case 3:
                strategy = "Benevolent player.";
                break;
            default:
                break;
            }
            cout << "Player [" << pl.at(i)->name << "] is a " << strategy << endl;
        }
        //set armies calling reinforcementPhase
        reinforcementPhase();
        //Players issue orders and place them in their order list 
        issueOrdersPhase();
        //the game engine proceeds to execute the top order 
        //on the list of orders of each player 
        executeOrdersPhase();

        // print cards owned by all players
        for (int i = 0; i < numOfPlayers; ++i)
        {
            notify(NULL, pl.at(i), NULL, "card");
        }

        string percentMsg = getPercentOfWorld();

        //call statistic observer
        notify(m, NULL, NULL, percentMsg);

        //then back to reinforcementPhase
        if (gameCheck())
        {
            //cout << "Player" << pl.at(0)->name << " has won the game!" << endl;
            notify(m, pl.at(0), NULL, "win"); //notify statistic observer the winner
            break;
        }
    }
    cout << "GAME OVER";
}

int main()
{
    // create a game engine object
    GameEngine* ge = new GameEngine();
    // execute the loading phase
    ge->loadingPhase();
    // execute the startup phase
    ge->startupPhase();
    // execute the main game loop
    ge->mainGameLoop();
    delete ge;
    ge = nullptr;

    return 0;
}