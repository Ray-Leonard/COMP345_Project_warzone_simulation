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

PhaseObserver& PhaseObserver::operator= (const PhaseObserver& src)
{
	_phaseSubject = new GameEngine(*src._phaseSubject);
	return *this;
}

void PhaseObserver::update(Map* m, Player* p, Order* o, string msg) {
	displayPhaseInfo(m, p, o, msg);
}

void PhaseObserver::displayPhaseInfo(Map* m, Player* p, Order* o, string msg) {
	if (m == nullptr) {
		cout << "<Phase Observer>" << endl;
		if (p->currentPhase == 2) {
			cout << "Player " << p->name << ": Reinforcrment phase." << endl;
			cout << "Player " << p->name << " is given " << msg << " armies/army." << endl;
			cout << "Player " << p->name << " now has " << p->armyPool << " armies/army in total." << endl << endl;
		}
		else if (p->currentPhase == 1) {
			if (msg.empty()) {
				cout << "Player " << p->name << ": Startup phase." << endl;
				cout << "Player " << p->name << " is given " << p->armyPool << " armies/army." << endl;
				cout << "Player " << p->name << " now has " << p->armyPool << " armies/army in total." << endl <<endl;
			} 
			else {
				cout << "All players: Startup phase." << endl;
				cout << "The player is now playing in the following order:" << msg << endl << endl;
				delete p;
			}
		}
		else if (p->currentPhase == 3) {
			cout << "Player " << p->name << ": Issue order phase." << endl;
			cout << "Player " << p->name << ": issues the following orders:" << endl;
			cout << *(p->orders);
			cout <<endl << endl;
		}
		else if (p->currentPhase == 4 && msg != "card") {
			cout << "Player " << (o->getPlayer())->name << ": Execute order phase." << endl;
			o->PrintMsg();
			cout << msg;
			cout << endl << endl;
		}
		else if (p->currentPhase == 4 && msg == "card") {
			if (p->cards->getNumOfCards() == 0) {
				cout << "Player [" << p->name << "] has no card in hand" << endl<<endl;
			} 
			else {
				cout << "Player [" << p->name << "] has the following cards in hand: " << *p->cards << endl<<endl;
			}
		}
	}
	//system("cls"); //clear the console
}

ostream& operator<< (ostream& output, PhaseObserver& phaseObs)
{

	output << "Phase Observer" << endl;
	return output;
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

void StatisticObserver::update(Map* m, Player* p, Order* o, string msg) {
	displayWorldDomination(m, p, o, msg);
}

StatisticObserver& StatisticObserver::operator= (const StatisticObserver& src)
{
	_statisticSubject = new GameEngine(*src._statisticSubject);
	return *this;
}

void StatisticObserver::displayWorldDomination(Map* m, Player* p, Order* o, string msg) {
	//StatisticObserver don't need phase observer
	if (m != nullptr) {
		cout << "\n\n<Statistic Observer>" << endl;
		if (msg == "win") {
			cout << "\n===========================================================" << endl;
			cout << "\t\t CONGRATULATIONS" << endl;
			cout << "Player [" << p->name << "] has conquered all territories!" << endl;
			cout << "Player [" << p->name << "] is winner!" << endl;
			cout << "===========================================================" << endl;
			cout << "\t\t GAME OVER" << endl;
		}
		else if (msg == "eliminate") {
			cout << "\n===========================================================" << endl;
			cout << "\t\t FAILED" << endl;
			cout << "Player [" << p->name << "] has lost all territories!" << endl;
			cout << "Player [" << p->name << "] will be eliminated from the game!" << endl;
			cout << "===========================================================" << endl<< endl;
		}
		else {
			//print territory
			cout << "========================== World Domination View ==========================" << endl;
			m->printNodes();
			cout << msg << endl;
			cout << "===========================================================================" << endl << endl;
		}
	}
}

ostream& operator<< (ostream& output, StatisticObserver& statisObs)
{

	output << "Statistic Observer" << endl;
	return output;
}


//Subject class
Subject::Subject() {
	_observers = new list<Observer*>;
}

Subject::~Subject() {
	delete _observers;
}

void Subject::attach(Observer* o) {
	_observers->push_back(o);
}

void Subject::detach(Observer* o) {
	_observers->remove(o);
}

void Subject::notify(Map* m, Player* p, Order* o, string msg) {
	list<Observer*>::iterator i = _observers->begin();
	for (; i != _observers->end(); ++i) {
		(*i)->update(m, p, o, msg);
	}
}
