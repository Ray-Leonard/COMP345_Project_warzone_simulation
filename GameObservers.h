#pragma once
#include "Player.h"
#include "Orders.h"
#include <list>

class GameEngine;

class Observer {
public:
	Observer();
	virtual ~Observer();
	virtual void update(string phase, Player* p, Order* o, string msg) = 0;
//protected:
};

// observe GameEngine. display phase & player info
class PhaseObserver : public Observer{
public:
	PhaseObserver(GameEngine* phaseSub);
	PhaseObserver(const PhaseObserver& phaseObserver);
	~PhaseObserver();
	void update(string phase, Player* p, Order* o, string msg);
	void displayPhaseInfo(string phase, Player* p, Order* o, string msg);
protected:
	GameEngine* _phaseSubject;
};

//observe Map. display territory & its owner info.
class StatisticObserver : public Observer{
public:
	StatisticObserver(GameEngine* statisticSub);
	StatisticObserver(const StatisticObserver& statisticObserver);
	~StatisticObserver();
	void update(string phase, Player* p, Order* o, string msg);
	void displayWorldDomination(string phase, Player* p, Order* o, string msg);
protected:
	GameEngine* _statisticSubject;
};

class Subject {
public: 
	virtual void attach(Observer* o);
	virtual void detach(Observer* o);
	virtual void notify(string phase, Player* p, Order* o, string msg);
	Subject();
	~Subject();
private:
	list<Observer*> *_observers;
};