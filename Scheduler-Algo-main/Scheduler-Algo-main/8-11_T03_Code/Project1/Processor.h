
#pragma once
#include<iostream>
#include"Queue.h"
#include <list>
#include "Process.h"
//#include "Scheduler.h"
using namespace std;
class Scheduler;
class Processor 
{
private:
    Process* CurrRun;
    Process* Terminate;  ///  termination from run 
    Process *Terminatekill;  // termination from killing 
    Process* BLK;
    char type;
    int RDY_Length;        //increasses when new process is added and decreases during run or steal etc.
    int BusyCount;     //incremented every time step during running
    int Total_CPUtime;  //increases when new process is added to rdy queue
    int Total_TRT;      //increases every time process is moved to the terminate list
   
protected:
   Scheduler * sc;
public:
    Processor();
   void setRDY_Length(int length);
     void addterminate(Process* pro)  //kill for FCFS
    {
         Terminatekill = pro;
    }
     Process* getkilltem() // get killed process to termination
    {
        return Terminatekill;
    }
    void addtoterminate(Process* p)  //rename to setTerminate after debugging
    {
        Terminate = p;
    }
    Process* getTerminated()
    {
        return Terminate;
    }
    //virtual void ReadyIDs() = 0;   // it fills qID AND CALLS print of DS
    void addtoBLK(Process* process)
    {
        BLK = process;
    }
    Process* getBLK()
    {
        return BLK;
    }

    //virtual void addtoBLK(Process*& process)
    //{
    //    if (getCurrRun() != NULL)
    //    {
    //       // if (getCurrRun()->getisBlocked())
    //            process = getCurrRun();
    //    }
    //}
    //virtual void addtoterminate(Process* & process)
    //{
    //    if (getCurrRun() != NULL)
    //    {
    //       //if (getCurrRun()->getisFinished())
    //          //  process = getCurrRun();
    //    }
    //    return;
    //}
   
    virtual int getRDYCount() = 0;
   // virtual Process* getNextProcess() = 0;       
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
   // virtual int readycount() = 0;
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

    ///*  bool isBusy() 
    //  {
    //      if (RDY_Length == 0) return false;
    //      else return true;
    //  }*/
    bool isBusy() // return true of currrun if working // dh feha 8lt need fix 
    {
        if (CurrRun) return true;
        return false;
    }

    void incrementBusyCount() {
        if (isBusy()) BusyCount++;
    }

    int getTotal_TRT() {
        return Total_TRT;
    }
    //friend ostream& operator<<(ostream& output, Processor * p1);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////me7tag as2al mo3ed 3ala haga hena / momken tet3ml btare2a tanya ashal b kteer gowa kol processor fa lama a4ofoko ha2oloko yarab afteker bs
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
     /*void unblockProcesses()
    {
        for ( Process * it = blockedProcesses.getHead(); it != nullptr; it = it->next) {
            if (it->isBlockedProcess()) {
               // if (it->decrementBlockedTime()) { // Check if blocked duration has ended also waiting for func decrement
                    addToReadyQueue(it);
                    it->next = nullptr; // Remove from the blocked queue
                    break; // Exit the loop after unblocking one element
                }
            }
        }
    }
    */

};
