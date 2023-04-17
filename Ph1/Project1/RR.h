#pragma once
#include "Processor.h"
#include "Queue.h"
#include <fstream>

using namespace std;
class RR : public Processor
{
private:
    Queue<Process*> RdyQueue;
    static int ts;    //Timeslice
    static int rtf;
    int Curtime;
public:
    RR();
    virtual void addToReadyQueue(Process* p1);
    void settimeslice(int t);
    virtual void ReadyIDs();
    virtual void ScheduleAlgo(int currenttimestep);
    
    bool CheckRTF(Process* p1);
    Process* getNextProcess(); 
    virtual void print_rdy();
    static void Loadp(ifstream& inputFile);


};
