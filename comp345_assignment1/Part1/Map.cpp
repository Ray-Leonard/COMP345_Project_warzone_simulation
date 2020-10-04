//
// Created by Nian Liu on 2020-10-02.
//

#include "Map.h"
#include <iostream>
#include <typeinfo>
#include <string>
using namespace std;

Territory::Territory()
{
    owner = new string("");
    tID = new int(0);
    production = new int(0);
    garrison = new int(0);
}
Territory::Territory(string playername, int nodeID)
{
    owner = new string("");
    *owner = playername;
    tID = new int(0);
    *tID = nodeID;
    production = new int(1);
    garrison = new int(0);
}
string Territory::getOwnership() {return *owner;}
void Territory::setOwner(string name){*owner=name;}
int Territory::getId() {return *tID;}
int Territory::getProduction() {return *production;}
int Territory::getGarrison() {return *garrison;}
void Territory::setGarrison(int army) {*garrison=army;}

Continent::Continent()
{
    continentName = new string("");
    numOfTerritory = new int(0);
}
Continent::Continent(string cName)
{
    continentName = new string("");
    *continentName = cName;
    numOfTerritory = new int(0);
}
string Continent::getContinentName() {return *continentName;}
int Continent::getNumOfMembers() {return *numOfTerritory;}
void Continent::addMember(Territory *t) {allMembers.push_back(t);}
void Continent::printMembers()
{
    for(int i=0;i<allMembers.size();i++)
    {
        cout<<(allMembers[i])->getOwnership()<<" ";
    }
    cout<<endl;
}
void Continent::setSubRelation(int **graph)
{
    int size = allMembers.size();
    subRelation = new int*[size];
    for(int i=0;i<size;i++)
    {
        subRelation[i] = new int[size];
    }
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            subRelation[i][j] = graph[i][j];
        }
    }
}
void Continent::printRelation()
{
    cout<<"\n-------------------"<<endl;
    int size = allMembers.size();
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            cout<<subRelation[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"-------------------"<<endl;
}

Map::Map()
{
    mapName = new string("");
}
Map::Map(string worldName)
{
    mapName = new string("");
    *mapName = worldName;
}
string Map::getMapName() {return *mapName;}
void Map::addContinent(Continent *c) {allContinents.push_back(c);}
void Map::addNode(Territory *tt) {allNodes.push_back(tt);}
void Map::printNodes()
{
    int size = allNodes.size();
    for(int i=0;i<size;i++)
    {
        cout<<allNodes[i]->getOwnership()<<endl;
    }
}
void Map::setAllRelation(int **allGraph)
{
    int size = allNodes.size();
    allRelation = new int*[size];
    for(int i=0;i<size;i++)
    {
        allRelation[i] = new int[size];
    }
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            allRelation[i][j] = allGraph[i][j];
        }
    }
}
void Map::printMap()
{
    cout<<"\n-------------------"<<endl;
    int size = allNodes.size();
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            cout<<allRelation[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"-------------------"<<endl;
}


