#pragma once
#include<iostream>
#include"Queue.h"
#include <list>
#include "Process.h"
using namespace std;
class Scheduler;
class Processor 
{
private:
    Process* CurrRun;
    char type;
    int RDY_Length;        //increases when new process is added and decreases during run or steal etc.
    int BusyCount;     //incremented every time step during running - bne3mel eh b deh??
    int Total_CPUtime;  //increases when new process is added to rdy queue
    int OverheatTime;    //the time left for the processor to return to operating after overheat 
    bool isOverheated;   //indicates whether the processor is overheated or not
   
protected:

   Scheduler * sc;  
   double TotalBusyTime; // time taken inside run for each processor
   double TotalIdleTime; // total busy time minus time step
   int currenttime; // time to check if it is at same time step

public:
    Processor();
    /////////////////////////////////////////
    //		Pure Virtual Functions	       //
    /////////////////////////////////////////
    virtual void Handle(int timestep) = 0;
    virtual char getPtype() = 0;   // returns a char that indicates the type of the processor 
    virtual Process* eject() = 0;  // used in overheat to remove processes from different ready lists
    virtual Process* gettop() = 0;// return top process in the list for stealing
    virtual double pUtil() = 0; // to measure the time of the processor when it is executing 
    virtual int getRDYCount() = 0;  // returns the number of processes in different ready lists
    virtual void addToReadyQueue(Process* process) = 0;  // adds process to different ready lists
    virtual void ScheduleAlgo(int time) = 0;  // the operating of each processor
    virtual void print_rdy() = 0;   // prints different ready lists
    /////////////////////////////////////////
    //				Getters				   //
    /////////////////////////////////////////
    Process* getCurrRun();
    double getTotalBusy();
    double getTotalIDLE();
    bool getisOverheated();
    int getOverheatTime();
    int getRDY_Length();
    char gettype();
    bool isBusy(); // return true of currrun if working // dh feha 8lt need fix 
    /////////////////////////////////////////
    //				setters				   //
    /////////////////////////////////////////
    void setisOverheated(bool b);
    void settype(char a);
    void setOverheatTime(int n);
    void setRDY_Length(int length);
   void setCurrRun(Process* p);
   /////////////////////////////////////////////////////////////////////////////////////////
    ~Processor();
};
