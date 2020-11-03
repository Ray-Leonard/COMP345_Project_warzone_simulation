//
// Created by Nian Liu on 2020-10-19.
//

#include "MapLoader.h"
using namespace std;

MapLoader::MapLoader() {
    filePath = "no file path";
    map_ptr = new Map();
}

MapLoader::MapLoader(string file_path) {
    filePath = file_path;
    map_ptr = new Map();
}

MapLoader::MapLoader(const MapLoader& ml_obj) {
    filePath = ml_obj.filePath;
    this->map_ptr = new Map(*(ml_obj.map_ptr));
}

MapLoader::~MapLoader() {
    delete map_ptr;
}

string MapLoader::getFilePath() { return filePath; }
void MapLoader::setFilePathPtr(string file_path) { filePath = file_path; }

istream& operator >> (istream& input, MapLoader& mapLoader_obj) {
    input >> mapLoader_obj.filePath; //store the input value into the filePath_ptr variable
    return input;
}

ostream& operator << (ostream& output, MapLoader& mapLoader_obj) {
    output << "The file name is " << mapLoader_obj.filePath << ". ";
    return output;
}

MapLoader& MapLoader::operator = (const MapLoader& ml_obj) {
    if (this != &ml_obj) {
        filePath = ml_obj.filePath;
        *map_ptr = *(ml_obj.map_ptr); //call the copy contructor in Map class
    }
    return *this;
}

/*
Load map file when file is valid.
Exit program when:
1. Wrong file extension
2. Fail to open file
3. Missing necessary sections like Continents, Countries and Borders
*/
Map* MapLoader::loadingMap() {
    /*
    check if the extension of filePath is .map or .txt, if not, exit the program
    size_t find (const string& sub_str, size_t pos = 0);
    if found, return index; if not found, return string::npos
    */
    if (filePath.find(".map") == string::npos && filePath.find(".txt") == string::npos) {
        cout << "Wrong file extension!" << endl;
        cout << "Invalid file, program exits." << endl;
        exit(0);
    }

    /*
    variables for creating Continent, Trritory object
    cNo: continent id
    cName: continent name
    bonusArmyNum: bonus army number if a play occupies all the territories/countries in current continent
    tName: territory/country name
    tNo: territory/country id
    t_cNo: continent id that a country/territory belongs to
    t_cName: continent name that a country/territory belongs to
    str: the variable for reading a single string
    c_ptr: continent obj pointer, used to add its territories
    line: string variable for read line by line
    hasContinents: check if the map file contains Continents section
    hasCountries: check if the map file contains Countries/Territories section
    hasBorders: check if the map file contains Borders section
    */
    int cNo = 0, tNo=0, t_cNo=0, bonusArmyNum;
    string str, cName, tName, t_cName;
    bool hasContinents = false, hasCountries = false, hasBorders = false;
    Continent* c_ptr;
    string line, cline;


    ifstream input(filePath);
    if (!input.fail()) { //succeed to read map file
        while (!input.eof()) {
            input >> str;
            //if the file reaches the continents part
            if (str == "[continents]") {
                //cout << "str now is: " << str << endl;
                hasContinents = true;
                while (str != "[countries]") {
                    input >> str; //read cName
                    if (input.eof()) {
                        cout << "End of file." << endl;
                        break;
                    }
                    if (str == "[countries]") {
                        //cout << "str now is: " << str <<endl;
                        break;
                    }
                    if (str == "[borders]") {
                        cout << "Invalid file content, missing Countries section!" << endl;
                        cout << "Invalid file, program exits." << endl;
                        exit(0);
                    }
                    cName = str;
                    input >> str; //read bonusArmyNum
                    bonusArmyNum = stoi(str); //convert to int
                    input >> str; //read color, ignore
                    Continent* c = new Continent(++cNo, cName, bonusArmyNum); //create a new continent obj
                    //cout << *c;
                    map_ptr->addContinent(c); //add continent to vector<Continent*> allContinents in Map obj
                    map_ptr->setNumOfContinents(map_ptr->getNumOfContinents() + 1); //increase the continent num in Map obj
                }
            }
            if (str == "[countries]") {
                if (!hasContinents) {
                    cout << "Invalid file content, missing Continents section!" << endl;
                    cout << "Invalid file, program exits." << endl;
                    exit(0);
                }
                hasCountries = true;
                while (str != "[borders]") {
                    //input >> str; //read tNo
                    if (input.eof()) {
                        cout << "End of file." << endl;
                        break;
                    }
                    string cStr;
                    vector<string> clineData; //store all the data of a line in country
                    int clineDataSize;
                    while (getline(input, cline)) {
                        //cout << "cline: " << cline << endl;

                        if (cline == "[borders]") {
                            str = cline;
                            //cout << "str now is: " << cline << endl;
                            break;
                        }
                        if (cline.empty()) { //check cline is ""
                            continue;
                        }
                        //break input into word using stringstream
                        stringstream cs(cline);
                        //push all the data in one line into vector
                        while (cs >> cStr) {
                            if (input.eof()) {
                                cout << "End of file." << endl;
                                break;
                            }
                            if (!cStr.empty()) {
                                clineData.push_back(cStr);
                            }
                        }
                        //assign date to variable and create Territory objects
                        clineDataSize = clineData.size(); //last two data is coordinate, ignore
                        for (int i = 0; i < clineDataSize - 2; i++) {
                            switch (i) {
                                case 0:
                                    tNo = stoi(clineData[0]); //store territory number
                                    break;
                                case 1:
                                    tName = clineData[1]; //store territory name
                                    break;
                                case 2:
                                    t_cNo = stoi(clineData[2]);
                                    break;
                                case 3: //check if one territory belongs to two continents
                                    cout << "One territory cannot belong to two continents." << endl;
                                    cout << "Invalid file, program exits." << endl;
                                    exit(0);
                                default:
                                    cout << "Invalid file, program exits." << endl;
                                    exit(0);
                            }
                        }
                        c_ptr = map_ptr->getContinentById(t_cNo); //get continent according to t_cNo
                        c_ptr->addTerritory(tNo); //add territory to corresponding continent
                        t_cName = c_ptr->getContinentName();
                        Territory* t = new Territory(tName, tNo, t_cNo, t_cName); //create a new territory obj
                        //cout << *t;
                        map_ptr->addNode(t); //add territory to vector<Territory*> allNodes in Map obj

                        map_ptr->setNumOfTrritories(map_ptr->getNumOfTrritories() + 1);//increase the territory num in Map obj
                        clineData.clear(); //reset the vector
                        cline = "";
                    }
                }
            }
            if (str == "[borders]") {
                if (!hasContinents || !hasCountries) {
                    cout << "Invalid file content, missing necessary section!" << endl;
                    cout << "Invalid file, program exits." << endl;
                    exit(0);
                }

                hasBorders = true;

                string tIdStr; // individual territory id string
                int tId; //individual territory id
                int count = 0; //used to get the first tid of current line
                Territory* mTT = new Territory(); //store the Territory pointer returned by getTerritoryById()
                Territory* adjTT = new Territory(); //store the adjacent Territory pointer

                //cout << "getNumOfTrritories: " << map_ptr->getNumOfTrritories() << endl;
                //map_ptr->printNodes(); //for test
                //while don't reach the end of file
                while (getline(input, line)) {
                    //cout << "line: " << line << endl;

                    //break input into word using stringstream
                    stringstream s(line);
                    while (s >> tIdStr) {
                        if (input.eof()) {
                            cout << "End of file." << endl;
                            break;
                        }
                        tId = stoi(tIdStr);
                        //cout << "line tId: " << tId << endl;
                        count++;

                        if (count == 1) {
                            mTT = map_ptr->getTerritoryById(tId);
                            //cout << "Trritory id: " << mTT->gettId() << endl;
                        }
                        else {
                            //adjTT = map_ptr->getTerritoryById(tId);
                            mTT->addAdjacentTerritory(tId); //add adjTT pinter to adjacentTerritoryVec vector of mTT
                        }
                    }
                    //for test
                    //cout << "print its adjacent territories: " << endl;
                    //mTT->printAdjacentTerritoryVec(); //for test

                    count = 0; //reset count to 0
                    line = "";
                }
            }
        }
        //cout << "hasBorders: " << hasBorders << endl; //test
        //cout << "hasContinents: " << hasContinents << endl; //test
        //cout << "hasCountries: " << hasCountries << endl; //test
        //if map file doesn't contain continents, countries, or borders section, show invalid file
        if (!hasBorders || !hasContinents || !hasCountries) {
            cout << "Invalid file content, missing necessary sections." << endl;
            cout << "Invalid file, program exits." << endl;
            exit(0);
        }
    }
    else { //fail to load map file
        cout << "Fail to load map file!" << endl;
        cout << "Invalid file, program exits." << endl;
        exit(0);
    }
    return map_ptr;
}
