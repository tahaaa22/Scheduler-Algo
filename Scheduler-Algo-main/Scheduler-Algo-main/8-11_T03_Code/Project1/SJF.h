#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    int TotalBusyTime; // time taken inside run for each processor
    int TotalIdleTime; // total busy time minus time step
    int TotalTRT; // total turn around time of all processes
    PriorityQueue<Process*> PQ;
    int curenttime;
    static char Ptype;

public:
    SJF(Scheduler *);
    virtual Process* gettop();
    virtual double pLoad();
    virtual double pUtil();
    void addToReadyQueue(Process* p1);
    char getPtype();
    virtual void ScheduleAlgo(int time);
    virtual void print_rdy();
    ////////////////////////////////////////Added by mimo////////////////////////////////////////
    void Loadp(ifstream& inputFile);
    virtual  int getRDYCount();
    Process* eject();
};
