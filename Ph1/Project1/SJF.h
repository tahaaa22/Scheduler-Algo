#pragma once
#include "Processor.h"
#include "PriorityQueue.h"
class SJF : public Processor
{
private:
    PriorityQueue<Process*> PQ;
    int curenttime;
public:
    void ScheduleAlgo(int time) 
    {
        curenttime = time;
        if (!PQ.isEmpty() && !getCurrRun()) {
            Process* temp;         //run empty and ready contains processes
            PQ.dequeue(temp);
            setCurrRun(temp);
        }
        else if (getCurrRun()) {     //run not empty
            getCurrRun()->execute(time);
            if (getCurrRun()->getisBlocked())
                getCurrRun()->setisBlocked(true);
        }
    }
    void addtoBLK(Process*& process)
    {
        if (getCurrRun()->getisBlocked())
            process = getCurrRun();
    }

    void addtoterminate(Process* & process)
    {
        if (getCurrRun()->getisFinished())
            process = getCurrRun();
    }
    Process* getNextProcess() {};
    void addToReadyQueue(Process* process) {};  // processor or scheduler
};
