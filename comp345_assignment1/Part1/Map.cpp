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
