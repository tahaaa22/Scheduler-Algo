#include "RR.h"
#include "Scheduler.h"
RR::RR()
{
    Curtime = 0;
    settype('r');
}

char RR::getPtype()
{
    return Ptype;
}

/* virtual int readycount()
    {
        return qID.getCount();
    }
 */
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

void RR::addToReadyQueue(Process* p1)
{
    RdyQueue.enqueue(p1);
    setRDY_Length(p1->gettimeRemaining() + getRDY_Length());
}

void RR::ScheduleAlgo(int timestep)
{
    //rdy queue msh empty w mafya4 current run bardo
    if (!RdyQueue.isEmpty() && !getCurrRun())
    {
        Process* temp;
        RdyQueue.dequeue(temp);
        setCurrRun(temp);
        Curtime = 0;
        setRDY_Length(getRDY_Length() - temp->getCpuTime()); //bn2s cpu time el fy el run

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
                sc->RuntoTrm(temp);
            }
        }

            // FROM RUN TO BLOCK
            if (!getCurrRun()->getIOqueue().isEmpty()) {

                if (temp->getIOqueue().peek().getFirstItem() == timestep) {
                    sc->RuntoBlk(temp);
                }
            }

            setCurrRun(nullptr);// RUN CURRENTLY EMPTY
        }
        else
        {
            getCurrRun()->execute(timestep); //a2alal el timestep
            Curtime++;
            // ADDDDD COND BLOCK
            if (!getCurrRun()->getIOqueue().isEmpty()) {

                if (getCurrRun()->getIOqueue().peek().getFirstItem() == timestep)
                {
                    sc->RuntoBlk(getCurrRun());
                    setCurrRun(nullptr);
                    Curtime = 0;
                }
            }
            else if (getCurrRun()->getisFinished())
            {
                sc->RuntoTrm(getCurrRun());
                setCurrRun(nullptr);
                Curtime = 0;

            }


        }

    }

void RR::print_rdy()
{
    RdyQueue.Print();

}
int RR::getRDYCount()
{
    return RdyQueue.getCount();
}

void RR::Loadp(ifstream& inputFile) {
    inputFile >> TS;
    inputFile >> RTF;

}
char RR::Ptype = 'r';

int RR::TS = 0;
int RR::RTF = 0;
