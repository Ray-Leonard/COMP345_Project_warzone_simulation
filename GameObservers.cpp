#include "GameObservers.h"
#include "GameEngine.h"
#include <iostream>
using namespace std;

//Observer class
Observer::Observer() {}
Observer::~Observer() {}





//PhaseObserver class
PhaseObserver::PhaseObserver(GameEngine* phaseSub) : Observer(), _phaseSubject(phaseSub){
	_phaseSubject->attach(this);
}

PhaseObserver::PhaseObserver(const PhaseObserver& phaseObserver) {
	_phaseSubject = new GameEngine(*phaseObserver._phaseSubject);
}

PhaseObserver::~PhaseObserver() {
	_phaseSubject->detach(this);
}

void PhaseObserver::update(string phase, Player* p, Order* o, string msg) {
	displayPhaseInfo(phase, p, o, msg);
}

void PhaseObserver::displayPhaseInfo(string phase, Player* p, Order* o, string msg) {
	cout << "<Phase Observer>" << endl;
	if (phase == "reinforcement") {
		cout << "Player " << p->name << ": Reinforcrment phase." << endl;
		cout << "Player " << p->name << " is given " << msg << " armies/army." << endl;
		cout << "Player " << p->name << " now has " << p->armyPool << " armies/army in total." << endl << endl;
	}
	else if (phase == "startup") {
		if (msg.empty()) {
			cout << "Player " << p->name << ": Startup phase." << endl;
			cout << "Player " << p->name << " is given " << p->armyPool << " armies/army." << endl;
			cout << "Player " << p->name << " now has " << p->armyPool << " armies/army in total." << endl;
			cout << "Player " << p->name << " owns the following territories: " << endl;
			p->printTerrOwn();
			cout << endl << endl;
		} 
		else {
			cout << "All players: Startup phase." << endl;
			cout << "The player is now playing in the following order:" << msg << endl << endl;
		}
	}
	else if (phase == "issueorder") {
		cout << "Player " << p->name << ": Issue order phase." << endl;
		cout << "Player " << p->name << ": issues the following orders:" << endl;
		cout << *(p->orders);
		//p->orders->getOrderList();
		cout <<endl << endl;
	}
	else if (phase == "execute") {
		cout << "Player " << (o->getPlayer())->name << ": Execute order phase." << endl;
		o->PrintMsg();
		cout << msg;
		cout << endl << endl;
	}
	//system("cls"); //clear the console
}






//StatisticObserver class
StatisticObserver::StatisticObserver(GameEngine* statisticSub) : Observer(), _statisticSubject(statisticSub) {
	_statisticSubject->attach(this);
}

StatisticObserver::~StatisticObserver() {
	_statisticSubject->detach(this);
}

StatisticObserver::StatisticObserver(const StatisticObserver& statisticObserver) {
	_statisticSubject = new GameEngine(*statisticObserver._statisticSubject);
}

void StatisticObserver::update(string phase, Player* p, Order* o, string msg) {
	//StatisticObserver don't need phase observer
	if (phase.empty()) {
	
	}
}
void StatisticObserver::displayWorldDomination(string phase, Player* p, Order* o, string msg) {}


//Subject class
Subject::Subject() {
	_observers = new list<Observer*>;
}

Subject::~Subject() {
	//delete every Observer* inside
	delete _observers;
}

void Subject::attach(Observer* o) {
	_observers->push_back(o);
}

void Subject::detach(Observer* o) {
	_observers->remove(o);
}

void Subject::notify(string phase, Player* p, Order* o, string msg) {
	list<Observer*>::iterator i = _observers->begin();
	for (; i != _observers->end(); ++i) {
		(*i)->update(phase, p, o, msg);
	}
}
