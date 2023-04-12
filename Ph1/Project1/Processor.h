#include<iostream>
#include"Queue.h"
#include <list>
#include "Process.h"
using namespace std;
class Processor 
{
private:
    Process* CurrRun;
    char type;
    int RDY_Length;        //increasses when new process is added and decreases during run or steal etc.
    int BusyCount;     //incremented every time step during running
    int Total_CPUtime;  //increases when new process is added to rdy queue
    int Total_TRT;      //increases every time process is moved to the terminate list 
    public:
    Queue<Process*> blockedProcesses; 
    Queue<Process*> terminatedProcesses; 

    Processor() //new
    {
        CurrRun = nullptr;
        RDY_Length = 0;
        BusyCount = 0;
        Total_CPUtime = 0;
        Total_TRT = 0;

    }
    virtual void addtoBLK(Process*& process){
          if (getCurrRun()->getisBlocked())
            process = getCurrRun();
    }
    virtual void addtoterminate(Process* & process){
          if (getCurrRun()->getisFinished())
            process = getCurrRun();
    }
    virtual Process* getNextProcess() = 0;       
    virtual void addToReadyQueue(Process* process) = 0;  // processor or scheduler
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
    
    int getRDY_Length() {
        return RDY_Length;
    }

    char gettype() {
        return type;
    }

    void settype(char a) {
        type = a;
    }

    void setCurrRun(Process* p)
    {     //new
        CurrRun = p;
    }
    Process* getCurrRun()
    {      //new 
        return CurrRun;
    }

    bool isBusy() {
        if (RDY_Length == 0) return false;
        else return true;
    }

    void incrementBusyCount() {
        if (isBusy()) BusyCount++;
    }

    int getTotal_TRT() {
        return Total_TRT;
    }


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
