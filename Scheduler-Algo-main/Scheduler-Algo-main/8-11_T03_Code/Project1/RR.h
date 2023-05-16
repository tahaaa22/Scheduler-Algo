#pragma once
#include "Processor.h"
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
class RR : public Processor
{
private:
   // int TotalBusyTime; // time taken inside run for each processor
   // int TotalIdleTime; // total busy time minus time step
   // int TotalTRT; // total turn around time of all processes
    static char Ptype;
    Queue<Process*> RdyQueue;
    static int TS;   //Timeslice
    static int RTF;  // used in migration 
    int Curtime;
    Scheduler* sc;
   
public:
    RR(Scheduler*);
    ~RR();
    void RDYlength();
    Process* eject();
    virtual Process* gettop();
    virtual double pUtil();
    char getPtype();
    virtual void addToReadyQueue(Process* p1);
    virtual void ScheduleAlgo(int timestep);
    virtual void print_rdy();
    virtual  int getRDYCount();
    void Handle(int timestep);
    static void Loadp(ifstream& inputFile);
};