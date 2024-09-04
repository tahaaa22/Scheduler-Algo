#pragma once
#include "Processor.h"
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
class RR : public Processor
{
private:
    static char Ptype; // char to check if we are in RR processor
    Queue<Process*> RdyQueue; // The Rdyqueue of the proccessor
    static int TS;   //Timeslice of the RR (time it should run in the processor before going back to rdy)
    static int RTF;  // used to check if the processor will migrate
    int Curtime; // holder to check when the timeslice of a process is finished 
    Scheduler* sc; //Pointer to schedular to access termination and blk 

public:
    RR(Scheduler*); // The constructor 

    //////////////////////////////////////GETTERS/////////////////////////////////////

    virtual Process* gettop(); // Returns the first element in the rdyqueue
    virtual char getPtype(); // returns the char which hold what type of processor we are currently in 
    virtual  int getRDYCount(); // Returns the length of the Rdyqueue

    ////////////////////////////////////Other fn//////////////////////////////////////

    virtual void OverheatRun(int overheatconst);  // Make the processor overheated 
    virtual Process* eject(); // if Overheated remove the process in this rdy list and add them to another

    ////////////////////////////////////
    /////        Virtual fn //////////// 
    ////////////////////////////////////


    virtual void ScheduleAlgo(int timestep); // Runs the process 
    virtual void Handle(int timestep); // check BLK,TRM,TS fn alone to avoid repetition called in scheulealgo 
    virtual double pUtil(); // Calculate the utilization of the processor
    virtual void addToReadyQueue(Process* p1, int time); // fn adds to the ready queue 
    virtual void print_rdy(); // Prints the rdyqueue
    static void Loadp(ifstream& inputFile); // Load the TS and RTF
    ~RR(); //destructor
};