#pragma once
#include "PriorityQueue.h"
#include "Processor.h"
class EDF : public Processor
{
private:
	PriorityQueue <Process*> EDFrdy;
	int curenttime;
	static char Ptype;
public:
	EDF();
	void ScheduleAlgo(int time);
	void addToReadyQueue(Process* p1);
	char getPtype();
	virtual void print_rdy();
	virtual int getRDYCount();
};