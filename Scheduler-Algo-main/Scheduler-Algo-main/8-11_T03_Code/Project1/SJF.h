#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    PriorityQueue<Process*> PQ;  //SJF ready list
    static char Ptype;

public:
    SJF(Scheduler *s);
    ~SJF();
    Process* eject();
    void Handle(int timestep);
    virtual Process* gettop();
    virtual double pUtil();
    void addToReadyQueue(Process* p1);
    char getPtype();
    virtual void ScheduleAlgo(int time);
    virtual void print_rdy();
    void Loadp(ifstream& inputFile);
    virtual  int getRDYCount();
};
