#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include <iostream>

#include <crtdbg.h>

using namespace std;

int main1() {

	cout << "testing player owns--------------------------\n";
	Player* p1ayer = new Player();
	cout << *p1ayer;	//stream insetipn operator

	//testing toDefend()
	cout << "testing toDefend() method--------------------------------\n";
	//p1ayer->toDefend();

	//testing toAttack()
	cout << "testing toAttack() method---------------------------------\n";
	//p1ayer->toAttack();


	cout << "testing issueOrder() method---------------------------------\n";
	//p1ayer->issueOrder();
	cout << *p1ayer;

	//delete player that created
	delete p1ayer;

	_CrtDumpMemoryLeaks();
	return 0;
}