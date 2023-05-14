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
    static int TS;    //Timeslice
    static int RTF;
    int Curtime;
    Scheduler* sc;
   
public:
    RR(Scheduler*);
    void RDYlength();
    Process* eject();
    virtual Process* gettop();
    virtual double pUtil();
    char getPtype();
    virtual void addToReadyQueue(Process* p1);
    virtual void ScheduleAlgo(int timestep);
    bool CheckRTF(Process* p1);
    virtual void print_rdy();
    virtual  int getRDYCount();
    void Handle(int timestep);
    ///////////////////////////////////////////////Addddded by mimo/////////////////////////////////////////
    static void Loadp(ifstream& inputFile);
};