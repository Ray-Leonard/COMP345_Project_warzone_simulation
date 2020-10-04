//
// Created by Nian Liu on 2020-10-02.
//

#ifndef DEMO11_MAP_H
#define DEMO11_MAP_H


#include <string>
//#include <iostream>
#include <list>
#include <array>
#include <vector>
using namespace std;

class Territory
{
    string* owner;
    int* tID;
    int* production;
    int* garrison;
public:
    Territory();
    Territory(string playerName,int nodeID);

    string getOwnership();
    void setOwner(string name);
    int getId();
    int getProduction();
    int getGarrison();
    void setGarrison(int army);

};

class Continent
{
    vector<Territory*> allMembers;
    string* continentName;
    int* numOfTerritory;
    int** subRelation;
public:
    Continent();
    Continent(string cName);

    string getContinentName();
    int getNumOfMembers();
    void addMember(Territory* t);
    void printMembers();
    void setSubRelation(int** graph);
    void printRelation();

};


#endif //DEMO11_MAP_H
