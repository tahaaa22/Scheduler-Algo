#include "EDF.h"
#include "Scheduler.h"

EDF::EDF(Scheduler * sch)
{
	settype('e');
	 TotalBusyTime = 0;
	 TotalIdleTime = 0;
	TotalTRT = 0;
	sc = sch;
}
Process* EDF::gettop()
{
	Process* p = NULL ;
	EDFrdy.peek(p);
	if (p&&(p->getorphanflag()))
		return p;
	else
	{
		EDFrdy.dequeue(p);
		return p;
	}
}
 double EDF::pLoad()
{
	return (TotalBusyTime / TotalTRT) ;

}
double EDF::pUtil()
{
	return (TotalBusyTime / (TotalBusyTime + TotalIdleTime)) ;
}
void  EDF::addToReadyQueue(Process* p1) //inserting a process to the RDY 
{
    if (EDFrdy.isEmpty() && !getCurrRun())
    {
        EDFrdy.enqueue(p1, p1->getDeadLine());
        setRDY_Length(getRDY_Length() + p1->getCpuTime());
    }
      else if (getCurrRun())
	  {
		if (p1->getDeadLine() < getCurrRun()->getDeadLine())
		{
            
			EDFrdy.enqueue(getCurrRun(), getCurrRun()->getDeadLine());
			setCurrRun(p1);
			setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining());
		}
        else
        {
            EDFrdy.enqueue(p1, p1->getDeadLine());
            setRDY_Length(getRDY_Length() + p1->getCpuTime());
        }
	  }

}

char  EDF::getPtype()
{
	return Ptype;
}

void EDF::ScheduleAlgo(int time)
{

    if (!EDFrdy.isEmpty() && !getCurrRun())
    {
        Process* temp;
        EDFrdy.dequeue(temp);
        if (temp->getfirsttime() == 0)
        {
            temp->setResponseTime(time - temp->getArrivalTime());
            temp->setfirsttime(1);
        }
        setCurrRun(temp);
        int IO_req = getCurrRun()->getIOqueue().peek().getFirstItem();
        getCurrRun()->setblktime(IO_req + time);
        setRDY_Length(getRDY_Length() - temp->getCpuTime());
    }
    else if (getCurrRun())
    {
        getCurrRun()->execute(time);
        TotalBusyTime++; // taha
        TotalIdleTime = time - TotalBusyTime; //taha
        if (!getCurrRun()->getIOqueue().isEmpty())
        {
            if (getCurrRun()->getblktime() == time)
            {
                getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
                sc->RuntoBlk(getCurrRun());
                /////////////taha///////////////////
                if (!EDFrdy.isEmpty()) //run empty and ready contains processes
                {
                    Process* temp;
                    EDFrdy.dequeue(temp);
                    setCurrRun(temp);
                    int IO_req = getCurrRun()->getIOqueue().peek().getFirstItem();
                    getCurrRun()->setblktime(IO_req + time);
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
            /////////////taha///////////////////
            if (!EDFrdy.isEmpty()) //run empty and ready contains processes
            {
                Process* temp;
                EDFrdy.dequeue(temp);
                setCurrRun(temp);
                int IO_req = getCurrRun()->getIOqueue().peek().getFirstItem();
                getCurrRun()->setblktime(IO_req + time);
            }
            //////////////////////////////////////////
        }
    }
}

void  EDF::print_rdy()
{
	EDFrdy.Print();
}

int  EDF::getRDYCount()
{
	return EDFrdy.getCount();
}
char EDF::Ptype = 'e';
