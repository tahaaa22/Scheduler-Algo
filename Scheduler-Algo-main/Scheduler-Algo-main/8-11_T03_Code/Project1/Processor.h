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
    //Process* Terminate;  ///  termination from run 
    //Process *Terminatekill;  // termination from killing 
    //Process* BLK;
    char type;
    int RDY_Length;        //increases when new process is added and decreases during run or steal etc.
    int BusyCount;     //incremented every time step during running - bne3mel eh b deh??
    int Total_CPUtime;  //increases when new process is added to rdy queue
    int OverheatTime;
    bool isOverheated;
   
protected:
   Scheduler * sc;
   //////////////////taha//////////////////////////////////////////////////
   //int TotalTRT;      //increases every time process is moved to the terminate list- 7asesny too much tafker w heya simple awy fa ba3melha fe scheduler
   double TotalBusyTime; // time taken inside run for each processor
   double TotalIdleTime; // total busy time minus time step
   int currenttime; // time to check if it is at same time step
 
   //////////////////////////////////////////////////////////////
public:
    Processor();
    virtual void Handle(int timestep) = 0;
    double getTotalBusy()
    {
        return TotalBusyTime;
    }
    double getTotalIDLE()
    {
        return TotalIdleTime;
    }

    void setisOverheated(bool b) 
    {
        isOverheated = b;
    }

    bool getisOverheated() 
    {
        return isOverheated;
    }

    virtual Process* eject() = 0;

    void setOverheatTime(int n) 
    {
        OverheatTime = n;
    }

    int getOverheatTime()
    {
        return OverheatTime;
    }
    virtual Process* gettop() = 0;// return top process in the list for stealing

   // virtual double pLoad() = 0; // to measure load balance for each processor - Will be romevedd wrong place (taha)

    virtual double pUtil() = 0; // to measure the time of the processor when it is executing 

   void setRDY_Length(int length)
   {
       RDY_Length = length;
   }

    // void addterminate(Process* pro)  //kill for FCFS
    //{
    //     Terminatekill = pro;
    //}

    // Process* getkilltem() // get killed process to termination
    //{
    //    return Terminatekill;
    //}

    //void addtoterminate(Process* p)  //rename to setTerminate after debugging
    //{
    //    Terminate = p;
    //}

    //Process* getTerminated()
    //{
    //    return Terminate;
    //}
   
    //void addtoBLK(Process* process)
    //{
    //    BLK = process;
    //}

    //Process* getBLK()
    //{
    //    return BLK;
    //}
   
    virtual int getRDYCount() = 0;  

    virtual void addToReadyQueue(Process *process) = 0;  // processor or scheduler

    virtual void ScheduleAlgo(int time) = 0;  //new

    virtual void print_rdy() = 0;

    void setCurrRun(Process* p)
    {     //new
        CurrRun = p;
    }

    Process* getCurrRun()
    {      //new 
        return CurrRun;
    }
  
    int getRDY_Length()
    {
        return RDY_Length;
    }

    char gettype() 
    {
        return type;
    }

    void settype(char a) 
    {
        type = a;
    }

    virtual char getPtype() = 0;

    
    bool isBusy() // return true of currrun if working // dh feha 8lt need fix 
    {
        if (CurrRun) return true;
        return false;
    }

    void incrementBusyCount() { // m4 me7tag anymore
        if (isBusy()) BusyCount++;
    }

   /* int getTotal_TRT() {
        return TotalTRT;
    }*/
  
};
