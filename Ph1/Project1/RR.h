#pragma once
#include "Processor.h"
#include "Queue.h"
class RR : public Processor
{
private:
    Queue<Process*> RdyQueue;
    int TS;    //Timeslice
    int RTF;
    int Curtime;
public:
    RR(int T=0)  
    {
        TS = T;
        Curtime = 0;
        settype('R');
    }
    virtual void addToReadyQueue(Process* process)
    {
        RdyQueue.enqueue(process);
    };
    void settimeslice(int t)
    {
        TS = t;
    };
    virtual void ScheduleAlgo(int currenttimestep)
    {
        if (!RdyQueue.isEmpty() && !getCurrRun()) {
            Process* temp;         
            RdyQueue.dequeue(temp);
            setCurrRun(temp);
        }

       /* if (Curtime == 0 && RdyQueue.getCount()>0)
        {
            Process* temp;
            RdyQueue.dequeue(temp);
            setCurrRun(temp);
            getCurrRun()->execute(currenttimestep);

        }
        else if (!getCurrRun())
        {
            if (Curtime + 1 == TS) // a5r sec
            {
                getCurrRun()->execute(currenttimestep);
                Curtime = 0;
                Process* temp= getCurrRun();
                int t = temp->gettimeRemaining();
                if (t > 0)
                {
                    RdyQueue.enqueue(temp);
                }
            }
            else
            {
                getCurrRun()->execute(currenttimestep);
                Curtime++;
            }
        }*/
            
            
            
       
    };
    bool CheckRTF(Process* p1);
    Process* getNextProcess() {
        if (RdyQueue.isEmpty()) return nullptr;
        else {
            Process* temp;
            RdyQueue.peek(temp);
            return temp;
        }
    }
    virtual void print_rdy()
    {
        RdyQueue.Print();
    }
    void Loadp() {
        return;
    }

};

