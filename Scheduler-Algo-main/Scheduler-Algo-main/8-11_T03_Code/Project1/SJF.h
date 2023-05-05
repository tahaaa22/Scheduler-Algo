#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    PriorityQueue<Process*> PQ;
    int curenttime;
    static char Ptype;
public:
    SJF();
    void addToReadyQueue(Process* p1);
    char getPtype();
    void ScheduleAlgo(int time);
    virtual void print_rdy();
    ////////////////////////////////////////Added by mimo////////////////////////////////////////
    void Loadp(ifstream& inputFile);
    virtual  int getRDYCount();
};
