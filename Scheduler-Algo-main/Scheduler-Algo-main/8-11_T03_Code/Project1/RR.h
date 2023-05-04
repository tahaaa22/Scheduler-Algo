#pragma once
#include "Processor.h"
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
class RR : public Processor
{
private:
    static char Ptype;
    //Queue <int> qID;
    Queue<Process*> RdyQueue;
    static int TS;    //Timeslice
    static int RTF;
    int Curtime;
public:
    RR();
    char getPtype();
    virtual void addToReadyQueue(Process* p1);
    virtual void ScheduleAlgo(int timestep);
    bool CheckRTF(Process* p1);
    virtual void print_rdy();
    virtual  int getRDYCount();
    static void Loadp(ifstream& inputFile);
};