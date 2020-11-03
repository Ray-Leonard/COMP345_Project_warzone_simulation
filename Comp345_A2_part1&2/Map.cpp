//
// Created by Nian Liu on 2020-10-02.
//

#include "Map.h"
#include <iostream>
#include <iomanip>
#include <stack>
#include <typeinfo>
#include <string>
using namespace std;

/* all data members in Territory */
Territory::Territory()
{
    tName = "";
    tID = 0;
    cID = 0;
    pID = 0;
    armyNum = 0;
    cName = "";
}
Territory::Territory(string tName, int tID, int cID, string cName)
{
    this->tName = tName;
    this->tID = tID;
    this->cID = cID;
    this->cName = cName;
    pID = 0;
    armyNum = 0;
}
Territory::Territory(const Territory& obj){
    tName = obj.tName;
    tID = obj.tID;
    cID = obj.cID;
    pID = obj.pID;
    armyNum = obj.armyNum;
    cName = obj.cName;
    adjacentTerritoryVec = obj.adjacentTerritoryVec;
    allAdjWithinSameConti = obj.allAdjWithinSameConti;
}
Territory::~Territory() {
    //delete adjacentTerritoryVec vector
    adjacentTerritoryVec.clear();
    allAdjWithinSameConti.clear();
}
string Territory::gettName() { return tName; }
int Territory::gettId() { return tID; }
int Territory::getcId() { return cID; }
int Territory::getpId() {return pID;}
int Territory::getArmyNum() { return armyNum; }
vector<int> Territory::getAdjacentTerritoryVec() { return adjacentTerritoryVec; }
void Territory::setName(string tName) { this->tName = tName; }
void Territory::settId(int tId) { tID = tId; }
void Territory::setcId(int cId) { cID = cId; }
void Territory::setpId(int pId) { pID = pId; }
void Territory::setArmyNum(int armyNum) { this->armyNum = armyNum; }
void Territory::addAdjacentTerritory(int adjT) {
    adjacentTerritoryVec.push_back(adjT);
}
vector<int> Territory::getAllAdjWithinSameConti() {
    return allAdjWithinSameConti;
}
void Territory::setAdjTrriWithinSameConti() {
    Territory* adjTerri;
    int adjCId; //continent id of adjacent continent

    for (vector<int>::iterator itt = adjacentTerritoryVec.begin(); itt != adjacentTerritoryVec.end(); ++itt) {
        adjTerri = Map::getTerritoryById(*itt);
        adjCId = adjTerri->getcId();
        if (adjCId == cID) {
            allAdjWithinSameConti.push_back(*itt);
        }
    }

}
void Territory::printAdjacentTerritoryVec() {
    //cout << "print all Adjacent Territory for tID: " << *tID_ptr << endl;
    int size = adjacentTerritoryVec.size();
    for (int i = 0; i < size; i++)
    {
        cout << adjacentTerritoryVec[i] << endl;
    }
}
void Territory::printAllAdjWithinSameConti() {
    cout << "print all Adjacent Territory within the same continent" << endl;
    int size = allAdjWithinSameConti.size();
    for (int i = 0; i < size; i++)
    {
        cout << allAdjWithinSameConti[i] << endl;
    }
}
ostream& operator <<(ostream& output, Territory& obj) {
    output.width(12);
    output << obj.tName;
    output << "\t" << obj.tID << "\t" << obj.pID << "\t\t" << obj.armyNum << "\t\t\t" << obj.cName <<endl;
    return output;
}

/* all data members in Continent */
Continent::Continent()
{
    continentName = "";
    cId = 0;
    bonusArmyNum = 0;
}
Continent::Continent(int cId, string cName, int bonusArmyNum)
{
    this->cId = cId;
    continentName = cName;
    this->bonusArmyNum = bonusArmyNum;
}
Continent::Continent(const Continent& obj){
    continentName = obj.continentName;
    cId = obj.cId;
    bonusArmyNum = obj.bonusArmyNum;
    allTerritories = obj.allTerritories;
}
Continent::~Continent() {
    //delete allTerritories vector
    allTerritories.clear();
}
string Continent::getContinentName() {return continentName;}
int Continent::getcId() { return cId; }
int Continent::getBounusArmyNum() { return bonusArmyNum; }
int Continent::getNumOfTerritories() { return allTerritories.size(); }
void Continent::setContinentName(string cName) { continentName = cName; }
void Continent::setcId(int cId) { this->cId = cId; }
void Continent::setBounusArmyNum(int bonusArmyNum) { this->bonusArmyNum = bonusArmyNum; }
void Continent::addTerritory(int t) { allTerritories.push_back(t);}

bool Continent::validateSubgraph() {
    //iterate allTerritories
    for (vector<int>::iterator it = allTerritories.begin(); it != allTerritories.end(); ++it) {
        //get map
        map<int, bool> tID_Visited = contiDFS(*it);//call dfs

        //iterate map and see if it contains false value
        for (map<int, bool>::iterator itt = tID_Visited.begin(); itt != tID_Visited.end(); itt++) {
            if (!itt->second) {
                return false;
            }
        }
    }
    return true;
}
map<int, bool> Continent::contiDFS(int start_node) {
    //create the tid-visited map and initialize all visited to false
    map<int, bool> tID_Visited;
    for (vector<int>::iterator it = allTerritories.begin(); it != allTerritories.end(); ++it) {
        tID_Visited.insert(pair<int, bool>(*it, false));
    }

    contiTraverseFunc(start_node, tID_Visited);
    return tID_Visited;
}
void Continent::contiTraverseFunc(int start_node, map<int, bool>& tID_Visited) {
    stack<int> s;
    s.push(start_node);

    while (!s.empty()) {
        int visitedNode = s.top();
        map<int, bool>::iterator iter;
        iter = tID_Visited.find(visitedNode);
        if (iter != tID_Visited.end()) {
            iter->second = true;
        }
        Territory* visitedNode_ptr = Map::getTerritoryById(visitedNode);
        visitedNode_ptr->setAdjTrriWithinSameConti();
        vector<int> visitedNode_adjacentVec = visitedNode_ptr->getAllAdjWithinSameConti();
        //visitedNode_ptr->printAllAdjWithinSameConti();
        s.pop();
        for (vector<int>::iterator it = visitedNode_adjacentVec.begin(); it != visitedNode_adjacentVec.end(); ++it) {
            iter = tID_Visited.find(*it);
            if (!iter->second && iter != tID_Visited.end()) {
                s.push(*it);
                iter->second = true; //mark neighbour as visited
            }
        }
    }
}
ostream& operator <<(ostream& output, Continent& obj) {
    output << "Continent cId: " << obj.cId << " cName: " << obj.continentName << " bonusArmyNum: " << obj.bonusArmyNum << endl;
    return output;
}

/* all data members in Map */
vector<Territory*> Map::allNodes;
vector<Continent*> Map::allContinents;

Map::Map()
{
    numOfTrritories = 0;
    numOfContinents = 0;
}
Map::Map(int terri_num, int conti_num) {
    numOfTrritories = terri_num;
    numOfContinents = conti_num;
}
Map::Map(const Map& obj){
    numOfTrritories = obj.numOfTrritories;
    numOfContinents = obj.numOfContinents;
}

Map::~Map() {
    //delete allNodes Vector
    for (Territory* node : allNodes)
    {
        if (node != nullptr) {
            node = nullptr;
            delete node;
        }
    }
    allNodes.clear();

    //delete allContinents Vector
    for (Continent* continent : allContinents)
    {
        if (continent != nullptr) {
            continent = nullptr;
            delete continent;
        }
    }
    allContinents.clear();
}
int Map::getNumOfTrritories() { return numOfTrritories; }
int Map::getNumOfContinents() { return numOfContinents; }
void Map::setNumOfContinents(int conti_num) { numOfContinents = conti_num; }
void Map::setNumOfTrritories(int terri_num) { numOfTrritories = terri_num; }
void Map::addContinent(Continent *c) {allContinents.push_back(c);}
void Map::addNode(Territory *tt) {allNodes.push_back(tt);}

Continent* Map::getContinentById(int cId) {
    //cout << "Get continent with id " << cId << ": " << *(allContinents[cId - 1]) << endl; //test
    return allContinents[cId - 1];
}

Territory* Map::getTerritoryById(int tID) {
    //cout << "Get territory with id " << tID << ": " << *(allNodes[tID - 1]) << endl; //test
    return allNodes[tID - 1];
}

//print all territories in this map
void Map::printNodes()
{
    cout.width(12);
    cout << "Territory" <<":";
    cout<<"\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
    int size = allNodes.size();
    for(int i=0;i<size;i++)
    {
        cout<<*(allNodes[i])<<endl;
    }
}

bool* Map::mapDFS(int start_node, const int nodes_num) {
    //create the visited array
    bool* visited = new bool[nodes_num];
    for (int i = 0; i < nodes_num; i++) {
        visited[i] = false; //initialize all the element to false
    }

    mapTraverseFunc(start_node, visited);
    return visited;
}

void Map::mapTraverseFunc(int start_node, bool visited[]) {
    //visited[start_node-1] = true; //the index of each node is one less than the actual territory id

    stack<int> s;
    s.push(start_node);

    while (!s.empty()) {
        int visitedNode = s.top();
        visited[visitedNode - 1] = true;
        Territory* visitedNode_ptr = Map::getTerritoryById(visitedNode);

        //if validate map, use adjacentTerritoryVec
        vector<int> visitedNode_adjacentVec = visitedNode_ptr->getAdjacentTerritoryVec();
        s.pop();
        for (vector<int>::iterator it = visitedNode_adjacentVec.begin(); it != visitedNode_adjacentVec.end(); ++it) {
            //cout << "*it is: " << *it << endl;
            if (!visited[*it - 1]) {
                s.push(*it);
                visited[*it - 1] = true; //mark neighbour as visited
            }
        }
    }


}

bool Map::isConnected(int nodes_num) {
    for (int i = 0; i < nodes_num; i++) {
        bool* visited = mapDFS(i + 1, nodes_num);

        for (int j = 0; j < nodes_num; j++) {
            if (!visited[j]) {
            	delete visited;
            	visited = nullptr;
                return false;
            }
        }
        delete visited;
    	visited = nullptr;
    }
    return true;
}

bool Map::validate() {
    //check if the map is connected
    bool isMapConnected = isConnected(numOfTrritories);
    if (!isMapConnected) {
        cout << "The map is not a connected graph." << endl;
        return false;
    }
    //check if continents are connected subgraph
    Continent* conti;
    bool isContinentConnected = true;
    bool isCurrentContinentConnected = false;
    for (vector<Continent*>::iterator it = allContinents.begin(); it != allContinents.end(); ++it) {
        conti = *it;
        isCurrentContinentConnected = conti->validateSubgraph();
        cout << "current continent id: " << conti->getcId() << " isConnected: " << isCurrentContinentConnected << endl;
        isContinentConnected = isContinentConnected && isCurrentContinentConnected;
    }
    if (!isContinentConnected) {
        cout << "One of the continents is not a connected subgraph." << endl;
        return false;
    }
    return true;
}
void Map::printGraph() {
    int size = numOfTrritories;
    int arr[size][size];
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            arr[i][j]=0;
        }
    }
    for(int i=0;i<size;i++){
        int f = (*(allNodes[i])).gettId();
        arr[i][f-1]=1;
        int j = (allNodes[i]->adjacentTerritoryVec).size();
        for(int n=0;n<j;n++){
            arr[i][(allNodes[i]->adjacentTerritoryVec)[n]-1]=1;
        }
    }
    cout<<"\tA1\tB2\tC3\tD4\tE5\tF6"<<endl;
    for(int n=0;n<size;n++){
        cout<<allNodes[n]->gettName()<<"\t";
        for(int j=0;j<size;j++){
            cout<<arr[n][j]<<"\t";
        }
        cout<<endl;
    }
}

ostream& operator <<(ostream& output, Map& map_obj) {
    //output << "This map has " << map_obj.numOfContinents << " continents and " << map_obj.numOfTrritories << " territories in total." << endl;
    output << "Print map: " << endl;
    output << "\t" << "Territory" << "\t" << "Id" << "\t" << "Player" << "\t" << "# of Army" << "\t" << "Continent" << endl;
    int size = map_obj.allNodes.size();
    for (int i = 0; i < size; i++)
    {
        output << *(map_obj.allNodes[i]) << endl;
    }
    return output;
}