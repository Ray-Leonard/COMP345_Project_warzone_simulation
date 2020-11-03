#include <iostream>

#include "GameEngine.h"


using namespace std;

int main() {
    GameEngine* ge = new GameEngine();
    //user input the path of the map, and save the map in GameEngine Map* m
    // /Users/nianliu/Desktop/Comp345/Comp335_Project/untitled/europe.map
    ge->readMap();
    //check map validation
    ge->m->validate();
    //check continents' number
    int k = ge->m->allContinents.size();
    cout<<k<<endl;
    //set up how many players that user wants to have, input number 2-5
    ge->setNumOfPlayer();
    cout<<"Players : "<<ge->getNumOfPlayer()<<endl;
    //set up whether need observer by user input (y/n)
    ge->setObserverSwitch();
    cout<<"Need Observer ? "<<ge->getObserverSwitch()<<endl;
    //random shuffle the vector<Player*>
    ge->printPlayerQ();
    ge->shufflePlayers();//random shuffle vector<Player*> by calling method
    cout<<"----------------------------------------"<<endl;
    ge->printPlayerQ();
    //test assign army & territories to players
    ge->startupPhase();
    ge->printPlayerArmy();
    //check territories default pid = 0
    ge->m->printNodes();
    //print all territories which are hold by each player
    for(int i=0;i<ge->numOfPlayers;i++){
        cout<<"Player["<<ge->pl[i]->getName()<<"] has territories:";
        ge->pl[i]->printTV();
    }



    return 0;
}