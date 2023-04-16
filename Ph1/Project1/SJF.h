#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    PriorityQueue<Process*> PQ;
    int curenttime;
public:
    SJF() {
        settype('s');
    }
    
    void addToReadyQueue(Process *p1) //inserting a process to the RDY 
    {
        PQ.enqueue(p1, p1->gettimeRemaining());
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
    void ScheduleAlgo(int prob) {
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
    virtual void ReadyIDs()
    {
        Process* temp;
        for (int i = 0; i < getRDY_Length(); i++)
        {
            PQ.dequeue(temp);
            int id = temp->getPID();
            qID.enqueue(id);
            PQ.enqueue (temp, temp->gettimeRemaining());
        }
        qID. Print();
    }
    
     Process* getNextProcess()
     {
        if (PQ.isEmpty()) return nullptr;
        else {
            Process* tmp;
            PQ.peek(tmp);
            return tmp;
        }
    }
     virtual void print_rdy()
    {
        PQ.Print();
    }
    
    void Loadp(ifstream & inputFile) {
        return;
    }
    
};
