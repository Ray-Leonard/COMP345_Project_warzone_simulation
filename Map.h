#pragma once
#include <string>
#include <list>
#include <array>
#include <map>
#include <vector>
using namespace std;

class Player;

class Territory
{
public:
    string tName; //territory name
    int tID; //territory id
    int cID; //continent id this territory belongs to
    string cName; //continent name this territory belongs to
    int pID; //player id
    int armyNum; //num of army 

    vector<int> adjacentTerritoryVec; //adjacent territories vector for current territory
    /*
    subgraph, contains relationship between territories in the current continent
    each element in allTerritories is a Territory obj, which contains adjacentTerritoryVec vector
    */
    vector<int> allAdjWithinSameConti;

    //default constructor
    Territory();

    //parameterized constructor
    Territory(string tName, int tID, int cID, string cName);
    //copy constructor
    Territory(const Territory& obj);
    //destructor
    ~Territory();

    string gettName();
    int gettId();
    int getcId();
    int getpId();
    int getArmyNum();
    vector<int> getAdjacentTerritoryVec();
    vector<int> getAllAdjWithinSameConti();
    void setAdjTrriWithinSameConti(); //set allAdjWithinSameConti
    void setName(string tName);
    void settId(int tId);
    void setcId(int cId);
    void setpId(int pId);
    void setArmy(int newarmyNum);
    void addArmy(int n);
    void removeArmy(int n);
    void addAdjacentTerritory(int t);
    void printAdjacentTerritoryVec();
    void printAllAdjWithinSameConti();

    friend ostream& operator <<(ostream& output, Territory& obj);
};

class Continent
{
public:
    string continentName;
    int cId; //continent id
    int bonusArmyNum; //# of army a player will get if he/she occupies all countries in current continent
    /*
    subgraph, contains relationship between territories in the current continent
    each element in allTerritories is a Territory obj, which contains adjacentTerritoryVec vector
    so allTerritories is a 2D vector
    note: the territories in adjacentTerritoryVec may exist in different continent
    */
    vector<int> allTerritories;

    Continent();
    Continent(int cId, string cName, int bonusArmyNum);
    Continent(const Continent& obj);
    ~Continent();

    vector<int>& getAllTerritories();
    string getContinentName();
    int getcId();
    int getBounusArmyNum();
    int getNumOfTerritories();
    void setContinentName(string cName);
    void setcId(int cId);
    void setBounusArmyNum(int bonusArmyNum);
    void addTerritory(int t);
    bool validateSubgraph();
    map<int, bool> contiDFS(int start_node);
    void contiTraverseFunc(int start_node, map<int, bool>& tID_Visited);

    friend ostream& operator <<(ostream& output, Continent& obj);

};

class Map
{
public:
    static string allMlMapNames[4]; //all maps in MapLoader class
    static string allCmMapNames[4]; //all maps in ConquestFileReader class
    static vector<Territory*> allNodes;
    static vector<Continent*> allContinents;
    int numOfTrritories;
    int numOfContinents;

    Map();
    Map(int terri_num, int conti_num);
    Map(const Map& obj);
    ~Map();
    static void printAllMapNames();
    int getNumOfTrritories();
    int getNumOfContinents();
    void setNumOfContinents(int conti_num);
    void setNumOfTrritories(int terri_num);
    void addContinent(Continent* c);
    void addNode(Territory* tt);
    static Continent* getContinentById(int cId);
    static Territory* getTerritoryById(int tID);

    void printNodes(); //print graph
    bool* mapDFS(int start_node, const int nodes_num); //pass the start node
    //node: the start node, visited: an array to mark if a node is visited or not, vec: adjacency vector
    void mapTraverseFunc(int start_node, bool visited[]);
    bool isConnected(int nodes_num); //nodes_num: total number of nodes

    //check if the map is connected graph
    bool validate();

    //assignment operator
    Map& operator=(const Map& map_obj);

    //output a table
    friend ostream& operator <<(ostream& output, Map& map_obj);
};