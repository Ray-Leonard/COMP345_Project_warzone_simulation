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
        cout << "Please input a correct map file name!" << endl;
        throw - 1;
    }

    int cNo = 0, tNo = 0, t_cNo = 0, bonusArmyNum;
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
                        cout << "Please input a correct map file" << endl;
                        throw - 1;
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
                    cout << "Please input a correct map file" << endl;
                    throw - 1;
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
                                cout << "Please input a correct map file" << endl;
                                throw - 1;
                            default:
                                cout << "Please input a correct map file" << endl;
                                throw - 1;
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
                    cout << "Please input a correct map file" << endl;
                    throw - 1;
                }

                hasBorders = true;

                string tIdStr; // individual territory id string
                int tId; //individual territory id
                int count = 0; //used to get the first tid of current line
                Territory* mTT = new Territory(); //store the Territory pointer returned by getTerritoryById()
                Territory* adjTT = new Territory(); //store the adjacent Territory pointer

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
                            
                        }
                        else {
                            
                            mTT->addAdjacentTerritory(tId); //add adjTT pinter to adjacentTerritoryVec vector of mTT
                        }
                    }

                    count = 0; //reset count to 0
                    line = "";
                }
            }
        }

        if (!hasBorders || !hasContinents || !hasCountries) {
            cout << "Invalid file content, missing necessary sections." << endl;
            cout << "Please input a correct map file" << endl;
            throw - 1;
        }
    }
    else { //fail to load map file
        cout << "Fail to load map file!" << endl;
        cout << "Please input a correct map file" << endl;
        throw - 1;
    }
    return map_ptr;
}



//adaptee
ConquestFileReader::ConquestFileReader() {
    fileName = "no file path";
    map_ptr = new Map();
}

ConquestFileReader::ConquestFileReader(string file_path) {
    fileName = file_path;
    map_ptr = new Map();
}

ConquestFileReader::ConquestFileReader(const ConquestFileReader& conMap) {
    fileName = conMap.fileName;
    this->map_ptr = new Map(*(conMap.map_ptr));
}

ConquestFileReader::~ConquestFileReader() {
    delete map_ptr;
}

istream& operator>>(istream& input, ConquestFileReader& conMap_obj) {
    input >> conMap_obj.fileName; //store the input value into the filePath_ptr variable
    return input;
}

ostream& operator<<(ostream& output, ConquestFileReader& conMap_obj) {
    output << "The file name is " << conMap_obj.fileName << ". ";
    return output;
}

ConquestFileReader& ConquestFileReader::operator=(const ConquestFileReader& conMap_obj) {
    if (this != &conMap_obj) {
        delete map_ptr;
        fileName = conMap_obj.fileName;
        *map_ptr = *(conMap_obj.map_ptr); //call assignment operator in Map class
    }
    return *this;
}


Map* ConquestFileReader::loadConquestMap() {
    if (fileName.find(".map") == string::npos && fileName.find(".txt") == string::npos) {
        cout << "Wrong file extension!" << endl;
        cout << "Please input a correct map file name!" << endl;
        throw - 1;
    }

    int cNo = 0, tNo = 0, t_cNo = 0, bonusArmyNum = 0;
    string str, cName, tName, t_cName;
    bool hasContinents = false, hasCountries = false;
    Continent* c_ptr;
    string line, cline;

    map<string, int> continets; // cName -> cID
    map<string, int> territories; // tName -> tID
    map<string, string> adjTerriStr; // tName -> adjacent territories name

    ifstream input(".\\conquestMap\\" + fileName);
    if (!input.fail()) { //succeed to read map file
        while (!input.eof()) {
            input >> str;
            //cout << "str now is: " << str << endl;

            //if the file reaches the continents part
            if (str == "[Continents]") {
                hasContinents = true;
                string delimiter = "=";

                while (str != "[Territories]") {
                    //read one line
                    getline(input, line);

                    if (input.eof()) {
                        cout << "End of file." << endl;
                        break;
                    }
                    if (line == "[Territories]") {
                        
                        str = line;
                        
                        break;
                    }
                    if (line.empty()) {
                        continue;
                    }

                    size_t delimiterPos = line.find(delimiter);
                    if (delimiterPos != string::npos) {
                        cName = line.substr(0, delimiterPos); //read continent name
                        line.erase(0, delimiterPos + 1);
                        bonusArmyNum = stoi(line); //convert to int
                    }
                    Continent* c = new Continent(++cNo, cName, bonusArmyNum); //create a new continent obj
                    continets.insert({ cName, cNo });
                    cout << *c;
                    map_ptr->addContinent(c); //add continent to vector<Continent*> allContinents in Map obj
                    map_ptr->setNumOfContinents(map_ptr->getNumOfContinents() + 1); //increase the continent num in Map obj
                }
            }

            if (str == "[Territories]") {
                if (!hasContinents) {
                    cout << "Invalid file content, missing Continents section!" << endl;
                    cout << "Please input a correct map file" << endl;
                    throw - 1;
                }

                hasCountries = true;
                string delimiter = ",";

                while (!input.eof()) {

                    while (getline(input, cline)) {
                        if (cline.empty()) {
                            continue;
                        }
                        //cout << "cline: " << cline << endl;

                        string belongsToContinent = "";
                        size_t delimiterPos = 0;
                        int countStr = 0; //0: tName, 1: num, 2: num, 3: cName, 4: adjConti

                        while (((delimiterPos = cline.find(delimiter)) != std::string::npos) && countStr < 5) {
                            switch (countStr) {
                            case 0:
                                tName = cline.substr(0, delimiterPos);
                                territories.insert({ tName, ++tNo });
                                cline.erase(0, delimiterPos + 1);
                                break;
                            case 1:
                            case 2:
                                cline.erase(0, delimiterPos + 1);
                                break;
                            case 3:
                                belongsToContinent = cline.substr(0, delimiterPos);
                                t_cNo = continets.at(belongsToContinent);
                                cline.erase(0, delimiterPos + 1);
                                break;
                            case 4:
                                adjTerriStr.insert({ tName, cline });
                                break;
                            default:
                                break;
                            }
                            countStr++;
                        }

                        c_ptr = map_ptr->getContinentById(t_cNo); //get continent according to t_cNo
                        c_ptr->addTerritory(tNo); //add territory to corresponding continent

                        Territory* t = new Territory(tName, tNo, t_cNo, belongsToContinent); //create a new territory obj
                        //cout << *t;

                        map_ptr->addNode(t); //add territory to vector<Territory*> allNodes in Map obj
                        map_ptr->setNumOfTrritories(map_ptr->getNumOfTrritories() + 1);//increase the territory num in Map obj
                        cline = "";
                    }
                }

                //add adjacent territory
                Territory* mTT = nullptr; //store the Territory pointer returned by getTerritoryById()
                size_t delimiterPos = 0;

                for (auto itr = adjTerriStr.begin(); itr != adjTerriStr.end(); ++itr) {
                    int mNo = territories.at(itr->first); //get key terri id
                    mTT = map_ptr->getTerritoryById(mNo);
                    string adjStr = itr->second; //contains all adj terri
                    string adjTerriStr = "";

                    //split adjStr into terri#
                    while ((delimiterPos = adjStr.find(delimiter)) != std::string::npos) {
                        adjTerriStr = adjStr.substr(0, delimiterPos);
                        int adjNo = territories.at(adjTerriStr); //get adjTerri id
                        mTT->addAdjacentTerritory(adjNo); //add adjTT pinter to adjacentTerritoryVec vector of mTT
                        adjStr.erase(0, delimiterPos + 1);
                    }
                    //add last adj terri to vector
                    adjTerriStr = adjStr;
                    int adjNo = territories.at(adjTerriStr);
                    mTT->addAdjacentTerritory(adjNo);


                }

            }
        }


        //if map file doesn't contain continents, countries, or borders section, show invalid file
        if (!hasContinents || !hasCountries) {
            cout << "Invalid file content, missing necessary sections." << endl;
            cout << "Please input a correct map file" << endl;
            throw - 1;
        }
    }
    else { //fail to load map file
        cout << "Fail to load map file!" << endl;
        cout << "Please input a correct map file" << endl;
        throw - 1;
    }
    return map_ptr;
}





ConquestFileReaderAdapter::ConquestFileReaderAdapter() : MapLoader(), cfr(nullptr) {}

ConquestFileReaderAdapter::ConquestFileReaderAdapter(ConquestFileReader* cfr_ptr) : MapLoader(), cfr(cfr_ptr) {}

ConquestFileReaderAdapter::~ConquestFileReaderAdapter() {
    delete cfr;
    cfr = nullptr;
}

Map* ConquestFileReaderAdapter::loadingMap() {
    cout << "Read conquest map." << endl;
    return cfr->loadConquestMap();
}