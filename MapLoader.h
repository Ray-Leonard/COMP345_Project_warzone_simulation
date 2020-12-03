#pragma once
#include <string>
#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//Target class
class MapLoader {
    string filePath;
    Map* map_ptr; //a Map obj

public:
    MapLoader();
    MapLoader(string file_path);
    MapLoader(const MapLoader& ml_obj); //copy constructor
    virtual ~MapLoader(); //destructor

    //getter
    string getFilePath();

    //setter
    void setFilePathPtr(string file_path);

    //load map and create Map obj, if map file is not valid, exit the program
    virtual Map* loadingMap();

    //assignment operator overloading
    MapLoader& operator = (const MapLoader& ml_obj);

    //input/output stream operator overloading
    friend istream& operator >> (istream& input, MapLoader& mapLoader_obj);
    friend ostream& operator << (ostream& output, MapLoader& mapLoader_obj);
};



//adaptee class
class ConquestFileReader {
    string fileName;
    Map* map_ptr; //a Map obj

public:
    ConquestFileReader();
    ConquestFileReader(string file_path);
    ConquestFileReader(const ConquestFileReader& conMap); //copy constructor
    ~ConquestFileReader(); //destructor

    //getter
    string getFileName() { return fileName; }

    //setter
    void setFilePathPtr(string file_path) { fileName = file_path; }

    //load conquest map and create Map obj, if map file is not valid, exit the program
    Map* loadConquestMap();

    //assignment operator overloading
    ConquestFileReader& operator=(const ConquestFileReader& conMap_obj);

    //input/output stream operator overloading
    friend istream& operator>>(istream& input, ConquestFileReader& conMap_obj);
    friend ostream& operator<<(ostream& output, ConquestFileReader& conMap_obj);
};


//adapter class
class ConquestFileReaderAdapter : public MapLoader {
    ConquestFileReader* cfr;
public:
    ConquestFileReaderAdapter();
    ConquestFileReaderAdapter(ConquestFileReader* cfr_ptr);
    ~ConquestFileReaderAdapter();
    virtual Map* loadingMap();
};
