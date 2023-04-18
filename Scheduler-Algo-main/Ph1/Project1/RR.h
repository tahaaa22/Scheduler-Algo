#pragma once
#include "Processor.h"
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
class RR : public Processor
{
private:
    static char Ptype;
    //Queue <int> qID;
    Queue<Process*> RdyQueue;
   static int TS;    //Timeslice
   static int RTF;
    int Curtime;
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
   /* virtual int readycount()
    {
        return qID.getCount();
    }*/
    virtual void addToReadyQueue(Process* p1)
    {
       
        RdyQueue.enqueue(p1);
       
    }
    void settimeslice(int t)
    {
        TS = t;
    }
  /*  virtual void ReadyIDs()
    {
        int n = RdyQueue.getCount();
        for (int i = 0; i < n; i++)
        {
            Process* temp;
            RdyQueue.dequeue(temp);
            int id = temp->getPID();
            qID.enqueue(id);
            RdyQueue.enqueue(temp);
        }

        qID.Print();
    }*/
    virtual void ScheduleAlgo(int time)
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
            
            
            
       
    }
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
   virtual  int getRDYCount()
    {
        return RdyQueue.getCount();
    }
    
    static void Loadp(ifstream& inputFile);
};


