#include <iostream>
#include "MapLoader.h"
using namespace std;

int main() {
	string file = "";
	cout << "please enter a file name: ";
	cin >> file;

	ConquestFileReader* cfr = new ConquestFileReader(file);
	cfr->loadConquestMap();

	return 0;
}