#include "SJF.h"
#include "Scheduler.h"
SJF::SJF(Scheduler * sch) 
{
    settype('s');
     TotalBusyTime = 0;
     TotalIdleTime = 0;
    TotalTRT = 0;
    sc = sch;
}
Process* SJF::gettop()
{
    Process* p= NULL ;
    PQ.peek(p);
    if (p && (p ->getorphanflag()))
    return p;
    else
    {
        PQ.dequeue(p);
        return p;
    }
 }
double SJF::pLoad()
{
    return (TotalBusyTime / TotalTRT) ;

}
double SJF::pUtil()
{
    return (TotalBusyTime / (TotalBusyTime + TotalIdleTime)) ;
}
void  SJF::addToReadyQueue(Process* p1) //inserting a process to the RDY 
{

    PQ.enqueue(p1, p1->gettimeRemaining());
    setRDY_Length(getRDY_Length() + p1->getCpuTime());
}
char  SJF::getPtype()
{
    return Ptype;
}

void  SJF::ScheduleAlgo(int time)
{
    
    if (!PQ.isEmpty() && !getCurrRun())
    {
        Process* temp;
        PQ.dequeue(temp);
        if (temp->getfirsttime() == 0)
        {
            temp->setResponseTime(time - temp->getArrivalTime());
            temp->setfirsttime(1);
        }
        setCurrRun(temp);
        setRDY_Length(getRDY_Length() - temp->getCpuTime());
    }
    else if (getCurrRun())
    {
        getCurrRun()->execute(time);
        TotalBusyTime++; // taha
        TotalIdleTime = time - TotalBusyTime; //taha
        if (!getCurrRun()->getIOqueue().isEmpty()) {

            if (getCurrRun()->getIOqueue().peek().getFirstItem() == time) {
                sc->RuntoBlk(getCurrRun());
                /////////////taha///////////////////
                if (!PQ.isEmpty()) //run empty and ready contains processes
                {
                    Process* temp;
                    PQ.dequeue(temp);
                    setCurrRun(temp);
                }
                //////////////////////////////////////////
            }
        }
        else if (getCurrRun()->getisFinished())
        {
            getCurrRun()->setTerminationTime(time);
            getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
            TotalTRT += getCurrRun()->getTurnaroundDuration(); //taha
            sc->Trm(getCurrRun());
            setCurrRun(nullptr);
        }
    }
}

void  SJF::print_rdy()
{
    PQ.Print();
}

void  SJF::Loadp(ifstream& inputFile)
{
    return;
}

int  SJF::getRDYCount()
{
    return PQ.getCount();
}


char SJF::Ptype = 's';
