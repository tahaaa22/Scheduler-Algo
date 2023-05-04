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

  
      virtual void ScheduleAlgo(int timestep, int  num)
    {
        //rdy queue msh empty w mafya4 current run bardo
        if (!RdyQueue.isEmpty() && !getCurrRun())
        {
            Process* temp;
            RdyQueue.dequeue(temp);
            setCurrRun(temp);
            Curtime = 0;
        }
        else if (getCurrRun())  //run not empty
        {
            Process* temp = getCurrRun();
            int t = temp->gettimeRemaining();
            if (Curtime == TS && t > 1) // a5r sec fy el ts
            {
                getCurrRun()->execute(timestep); // bt2alal timestep b 1
                Curtime = 0;

                // BACK TO RDY QUEUE 
                if (t > 0) // process time slice bt3ha 5las 5ls 
                {
                    RdyQueue.enqueue(temp);
                }

                // FROM RUN TO TERMINATION
                if (t == 0)  
                {
                    addtoterminate(temp); // calling fn el schedular
                }

                // FROM RUN TO BLOCK

                setCurrRun(nullptr);// RUN CURRENTLY EMPTY
            }
            else
            {
                getCurrRun()->execute(timestep); //a2alal el timestep
                Curtime++;
                if (t == 0) // cpu time 5ls 5las bardo 
                {
                    addtoterminate(temp); // calling fn el schedular
                    setCurrRun(nullptr);
                    Curtime = 0;
                }
                // ADDDDD COND BLOCK


            }

        }
        ////rdy queue msh empty w mafya4 current bardo
       //if (!RdyQueue.isEmpty() && !getCurrRun()) 
       //{
       //    Process* temp;         
       //    RdyQueue.dequeue(temp);
       //    setCurrRun(temp);
       //}
       //else if (getCurrRun())  //run not empty
       //{
       //    if (time <= 15)
       //    {
       //        Process* P = getCurrRun();
       //       P->setisBlocked(true);
       //        addtoBLK(P);
       //        setCurrRun(nullptr);
       //    }
       //    else if (time >= 20 && time <= 30)
       //    {
       //        addToReadyQueue(getCurrRun());
       //        setCurrRun(nullptr);
       //    }
       //    else if (time >= 50 && time <= 60)
       //    {
       //        Process* P = getCurrRun();
       //        addtoterminate( P);
       //        setCurrRun(nullptr);
       //    }
       //}
    }

    bool CheckRTF(Process* p1);
   /* Process* getNextProcess() {
        if (RdyQueue.isEmpty()) return nullptr;
        else {
            Process* temp;
            RdyQueue.peek(temp);
            return temp;
        }
    }*/
    virtual void print_rdy()
    {
        RdyQueue.Print();

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


