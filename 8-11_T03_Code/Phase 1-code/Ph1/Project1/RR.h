#pragma once
#include "Processor.h"
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
class RR : public Processor
{
private:
    static char Ptype; // phase 2 - type instead of dynamic casting 
    Queue<Process*> RdyQueue;
   static int TS;    //Timeslice
   static int RTF; // used in phase 2
    int Curtime; // used in phase 2 - increment as holder in run 
public:
    RR(int t=0, int rtf=0)  
    {
        Curtime = 0;   
        settype('r');
    }
    char getPtype()
    {
        return Ptype;
    }
 
    virtual void addToReadyQueue(Process* p1)
    {
        RdyQueue.enqueue(p1);
    }

  
    virtual void ScheduleAlgo(int time, int  num) // editable in phase 2
    {
        //rdy queue msh empty w mafya4 current bardo
        if (!RdyQueue.isEmpty() && !getCurrRun()) 
        {
            Process* temp;         
            RdyQueue.dequeue(temp);
            setCurrRun(temp);
        }
        else if (getCurrRun())  //run not empty
        {
            if (time <= 15)
            {
                Process* P = getCurrRun();
               P->setisBlocked(true);
                addtoBLK(P);
                setCurrRun(nullptr);
            }
            else if (time >= 20 && time <= 30)
            {
                addToReadyQueue(getCurrRun());
                setCurrRun(nullptr);
            }
            else if (time >= 50 && time <= 60)
            {
                Process* P = getCurrRun();
                addtoterminate( P);
                setCurrRun(nullptr);
            }
        }
       /* if (Curtime == 0 && RdyQueue.getCount()>0) // for phase 2 stuff related to shceduleralgo
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
    }

    virtual  int getRDYCount()
    {
        return RdyQueue.getCount();
    }
    bool CheckRTF(Process* p1); // checking rtf in phase 2
   
    virtual void print_rdy()
    {
        RdyQueue.Print();

    }

    static void Loadp(ifstream& inputFile);
};


