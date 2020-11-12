//
// Created by Nian Liu on 2020-10-19.
//

#ifndef DEMO11_MAPLOADER_H
#define DEMO11_MAPLOADER_H


#pragma once
#include <string>
#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class MapLoader {
    //string* filePath_ptr[3]; //the map file path
    string filePath;
    Map* map_ptr; //a Map obj

public:
    MapLoader();
    MapLoader(string file_path);
    MapLoader(const MapLoader& ml_obj); //copy constructor
    ~MapLoader(); //destructor

    //getter
    string getFilePath();

    //setter
    void setFilePathPtr(string file_path);

    //load map and create Map obj, if map file is not valid, exit the program
    Map* loadingMap();

    //assignment operator overloading
    MapLoader& operator = (const MapLoader& ml_obj);

    //input/output stream operator overloading
    friend istream& operator >> (istream& input, MapLoader& mapLoader_obj);
    friend ostream& operator << (ostream& output, MapLoader& mapLoader_obj);
};


#endif //DEMO11_MAPLOADER_H
