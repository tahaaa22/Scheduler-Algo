#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    PriorityQueue<Process*> PQ;  //SJF ready list
    static char Ptype;  // character set as 's' to indicate the type is SJF

public:
    SJF(Scheduler* s);  // constructor 
    ~SJF();  // destructor 
    virtual Process* eject();  // dequeues from the priority queue
    virtual void Handle(int timestep);  // executes the process in the run state and 
    virtual void OverheatRun(int overheatconst);  // adds the process in the run state to the shortest ready list (called if the processor overheats)
    virtual Process* gettop();  // returns the process on the head for the steal function 
    virtual double pUtil();  // calculates the utilization percentage of the processor 
    virtual void addToReadyQueue(Process* p1, int time);  // adds a process to the ready list 
    virtual char getPtype();  // getter for the data member Ptype
    virtual void ScheduleAlgo(int time);  // operates the processor 
    virtual void print_rdy();  // prints the processes in the ready list
    virtual int getRDYCount();  // returns number of processes in the ready list
};