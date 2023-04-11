#include<iostream>
#include"Queue.h"
#include <list>
#include "Process.h"
using namespace std;
class Processor 
{
private:
    Process* CurrRun;      //new
    public:
    Queue<Process*> blockedProcesses; 
    Queue<Process*> terminatedProcesses; 

    Processor() //new
    {
        CurrRun = nullptr;

    }
    virtual void addtoBLK(Process*& process) = 0;
    virtual void addtoterminate(Process* & process) = 0;
    virtual Process* getNextProcess() = 0;       
    virtual void addToReadyQueue(Process* process) = 0;  // processor or scheduler
    virtual void ScheduleAlgo(int time) = 0;  //new
    void setCurrRun(Process* p)
    {     //new
        CurrRun = p;
    }
    Process* getCurrRun()
    {      //new 
        return CurrRun;
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