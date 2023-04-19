#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    PriorityQueue<Process*> PQ;
    int curenttime;
    static char Ptype ; // for returning type instead of dynamic casting 

public:
    SJF() {
        settype('s'); 
    }
 
    void addToReadyQueue(Process *p1) //inserting a process to the RDY 
    {
      
        PQ.enqueue(p1, p1->gettimeRemaining());
 
        
    }
    char getPtype() // return type of sjf
    {
        return Ptype;
    }
    
   /* void ScheduleAlgo(int time)
    {
        curenttime = time;
        if (!PQ.isEmpty() && !getCurrRun()) {
            Process* temp;         //run empty and ready contains processes
            PQ.dequeue(temp);
            setCurrRun(temp);
        }
        /*
        else if (getCurrRun()) {     //run not empty
            getCurrRun()->execute(time);
        }
        */
 //   }
    void ScheduleAlgo(int prob, int  num) // mainly readty to run for phase 1 editable in phase 2
    {
        if (!PQ.isEmpty() && !getCurrRun()) {
            Process* temp;
            PQ.dequeue(temp);
            setCurrRun(temp);
        }
        else if (getCurrRun())
        {
            if (prob >= 1 && prob <= 15) {
                Process* temp = getCurrRun();
                addtoBLK(temp);
                setCurrRun(nullptr);
            }
            else if (prob >= 20 && prob <= 30) {
                Process* temp = getCurrRun();
                addToReadyQueue(temp);
                setCurrRun(nullptr);
            }
            else if (prob >= 50 && prob <= 60) {
                Process* temp = getCurrRun();
                addtoterminate(temp);
                setCurrRun(nullptr);
            }
        }
    }
    virtual  int getRDYCount()
    {
        return PQ.getCount();
    }


     virtual void print_rdy() 
    {
        PQ.Print();
    }
    
    
};
