#include <iostream>
#include "MapLoader.h"
using namespace std;

int main() {
	string file = "";
	cout << "please enter a file name: ";
	cin >> file;

	//test adaptee
	//ConquestFileReader* cfr = new ConquestFileReader(file);
	//cfr->loadConquestMap();
	//delete cfr;
	//cfr = nullptr;

	//test adaptor
	ConquestFileReaderAdapter* cfm = new ConquestFileReaderAdapter(new ConquestFileReader(file));
	cfm->loadingMap();
	delete cfm;
	cfm = nullptr;

	return 0;
}