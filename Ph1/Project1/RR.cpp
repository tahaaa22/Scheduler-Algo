#include "RR.h"
#include <fstream>
#include <iostream>
#include <istream>
using namespace std;

RR::RR()
{
    Curtime = 0;
    settype('r');
}
 void RR::addToReadyQueue(Process* p1)
{
    RdyQueue.enqueue(p1);
}
void RR::settimeslice(int t)
{
    ts = t;
}
void RR::ReadyIDs()
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
}
 void  RR::ScheduleAlgo(int currenttimestep)
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




}
 Process* RR::getNextProcess() {
     if (RdyQueue.isEmpty()) return nullptr;
     else {
         Process* temp;
         RdyQueue.peek(temp);
         return temp;
     }
 }
  void  RR::print_rdy()
 {
     RdyQueue.Print();

 }


  void RR::Loadp(ifstream& inputFile) {
     inputFile >> ts;
     inputFile >> rtf;

 }

  int RR::ts = 0;
  int RR::rtf = 0;
