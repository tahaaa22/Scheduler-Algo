#pragma once
#include "PriorityQueue.h"
#include "Processor.h"
class EDF : public Processor
{
private:
	int TotalBusyTime; // time taken inside run for each processor
	int TotalIdleTime; // total busy time minus time step
	int TotalTRT; // total turn around time of all processes
	PriorityQueue <Process*> EDFrdy;
	int curenttime;
	static char Ptype;
public:
	EDF(Scheduler *);
	virtual Process* gettop() ;
	virtual double pLoad();
	virtual double pUtil();
	virtual void ScheduleAlgo(int time);
	void addToReadyQueue(Process* p1);
	char getPtype();
	virtual void print_rdy();
	virtual int getRDYCount();
	Process* eject();
};