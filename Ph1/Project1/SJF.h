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
    
    void addToReadyQueue(Process* process) //inserting a process to the RDY 
    {
        PQ.enqueue(process, process->gettimeRemaining());
    }
    
    void ScheduleAlgo(int time) 
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
    }
    
     Process* getNextProcess() {
        if (PQ.isEmpty()) return nullptr;
        else {
            Process* tmp;
            PQ.peek(tmp);
            return tmp;
        }
    }
    
    void Loadp() {
        return;
    }
    
};
