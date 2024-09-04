#pragma once
#include "PriorityQueue.h"
#include "Processor.h"
class EDF : public Processor
{
private:
	PriorityQueue <Process*> EDFrdy;  // EDF ready list
	static char Ptype;  // character set as 'e' to indicate the type is EDF
public:
	EDF(Scheduler*);  // constructor
	virtual Process* eject();  // dequeues from the priority queue
	virtual void OverheatRun(int overheatconst);  // adds the process in the run state to the shortest ready list (called if the processor overheats)
	virtual void Handle(int timestep);  // executes the process in the run state and 
	virtual Process* gettop();  // returns the process on the head for the steal function 
	virtual double pUtil();  // calculates the utilization percentage of the processor 
	virtual void ScheduleAlgo(int time);  // operates the processor 
	virtual void addToReadyQueue(Process* p1, int time);  // adds a process to the ready list 
	virtual char getPtype();  // getter for the data member Ptype
	virtual void print_rdy();  // prints he processes in the ready list
	virtual int getRDYCount();  // returns number of processes in the ready list
	~EDF();  // destructor 
};