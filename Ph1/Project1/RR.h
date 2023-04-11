#pragma once
#include "Processor.h"
#include "Queue.h"
class RR : public Processor
{
private:
    Queue<Process*> PQ;
    int TS;    //Timeslice
public:
    RR(int T=0)  
    {
        TS = T;
    }
    virtual void addToReadyQueue(Process* process)
    {
        PQ.enqueue(process);
    };
    void settimeslice(int t)
    {
        TS = t;
    };
    //lw msh ha request input/output 
    //need tt3dal lw htd5ol block fy nos el timeslice yba i need el hba el ht3odhom bs
    virtual void execute(int &currenttimestep)
    {
        //dayman hena el RUN b null;
        //LW AWL PROCESS FA MAFYA4 HAGA W B3D EL TS BRG3O B NULL
        //lw mafya4 blocked up time
        if (PQ.getCount() > 0) // lw while msh ht5rog 5er w kolo terminated
        {
            Process* temp;         
            PQ.dequeue(temp);
            setCurrRun(temp); 
            int t = temp->gettimeRemaining();
            if (t - TS > 0) 
            {
                _sleep(TS); // msh lazm a sleep bgd sa7?
                temp->settimeRemaining(t - TS);
                setCurrRun(NULL);
                PQ.enqueue(temp);
                currenttimestep = currenttimestep + TS;
            }
            else 
                // lw fadlha a2a2l mn ts msh ha3od m3tla el program kol dh y3ny  
            {
                _sleep(t);
                temp->settimeRemaining(0);
                setCurrRun(NULL);
                getCurrRun()->execute(currenttimestep); // bsgal el terminated time bt3ha
                addTerminatedProcess(temp);
                currenttimestep = currenttimestep + t;
            }
        
   
        }
    };

};

