#include "RR.h"
#include "Scheduler.h"
RR::RR(Scheduler * sch)
{
    Curtime = 0;
    settype('r');
    TotalBusyTime = 0;
    TotalIdleTime = 0;
    TotalTRT = 0;
    sc = sch;
}
Process* RR::gettop()
{
    Process* p = NULL;
    RdyQueue.peek(p);
    if (p &&(p->getorphanflag()))
        return p;
    else
    {
        RdyQueue.dequeue(p);
        return p;
    }
}
double RR::pLoad()
{
    return (TotalBusyTime / TotalTRT);

}
double RR::pUtil()
{
    return (TotalBusyTime / (TotalBusyTime + TotalIdleTime));
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


void RR::addToReadyQueue(Process* p1)
{
    RdyQueue.enqueue(p1);
    setRDY_Length(p1->gettimeRemaining() + getRDY_Length());
}

void RR::ScheduleAlgo(int timestep)
{
    if (getisOverheated())
    {
        setOverheatTime(getOverheatTime() - 1);
        if (getOverheatTime() == 0) setisOverheated(false);
    }
    //rdy queue msh empty w mafya4 current run bardo
    else if (!RdyQueue.isEmpty() && !getCurrRun())
    {
        Process* temp;
        RdyQueue.dequeue(temp);
        if (temp->getfirsttime() == 0)
        {
            temp->setResponseTime(timestep - temp->getArrivalTime());
            temp->setfirsttime(1);
        }
        setCurrRun(temp);
        Curtime = 0;
         setRDY_Length(getRDY_Length() - temp->getCpuTime()); //bn2s cpu time el fy el run

    }

    else if (getCurrRun())  //run not empty
    {
        Process* temp = getCurrRun();
        int t = temp->gettimeRemaining();

        bool migrate = true;
        while (migrate)
        {
            migrate = sc->migrationrtf(temp, RTF);
            if (migrate == true)
            {
                if (!RdyQueue.isEmpty()) //run empty and ready contains processes
                {
                    Process* temp;
                    RdyQueue.dequeue(temp);
                    setCurrRun(temp);
                }
            }
        }
        if (Curtime == TS && t > 1) // a5r sec fy el ts
        {
            getCurrRun()->execute(timestep); // bt2alal timestep b 1
            TotalBusyTime++;
            TotalIdleTime = timestep - TotalBusyTime;
            Curtime = 0;

            // BACK TO RDY QUEUE 
            if (t > 0) // process time slice bt3ha 5las 5ls 
            {
                RdyQueue.enqueue(temp);
            }

            // FROM RUN TO TERMINATION
            if (t == 0)
            {
                temp->setTerminationTime(timestep);
                temp->setTurnaroundDuration(temp->getTerminationTime() - temp->getArrivalTime());
                TotalTRT += temp->getTurnaroundDuration();
                sc->Trm(temp);
            }
            // FROM RUN TO BLOCK
            if (!getCurrRun()->getIOqueue().isEmpty())
            {

                if (temp->getIOqueue().peek().getFirstItem() == timestep)
                {
                    sc->RuntoBlk(temp);
                }
            }
            if (!RdyQueue.isEmpty()) //run empty and ready contains processes
            {
                Process* temp;
                RdyQueue.dequeue(temp);
                setCurrRun(temp);
            }

        }
        else
        {
            getCurrRun()->execute(timestep); //a2alal el timestep
            TotalBusyTime++;
            TotalIdleTime = timestep - TotalBusyTime;
            Curtime++;
            // ADDDDD COND BLOCK
            if (!getCurrRun()->getIOqueue().isEmpty())
            {

                if (getCurrRun()->getIOqueue().peek().getFirstItem() == timestep)
                {
                    sc->RuntoBlk(getCurrRun());
                    if (!RdyQueue.isEmpty()) //run empty and ready contains processes
                    {
                        Process* temp;
                        RdyQueue.dequeue(temp);
                        setCurrRun(temp);
                    }
                    Curtime = 0;
                }
            }
            else if (getCurrRun()->getisFinished())
            {
                getCurrRun()->setTerminationTime(timestep);
                getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
                TotalTRT += getCurrRun()->getTurnaroundDuration();
                sc->Trm(getCurrRun());
                if (!RdyQueue.isEmpty()) //run empty and ready contains processes
                {
                    Process* temp;
                    RdyQueue.dequeue(temp);
                    setCurrRun(temp);
                }
                Curtime = 0;

            }


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

Process* RR::eject() {
    Process* temp;
    RdyQueue.dequeue(temp);
    return temp;
}

char RR::Ptype = 'r';

int RR::TS = 0;
int RR::RTF = 0;
