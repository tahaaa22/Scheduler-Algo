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
	if (p1 && getCurrRun())
	{
		if (p1->getDeadLine() < getCurrRun()->getDeadLine())
		{
			EDFrdy.enqueue(getCurrRun(), getCurrRun()->getDeadLine());
			setCurrRun(p1);
			setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining());
		}
		EDFrdy.enqueue(p1, p1->getDeadLine());
		setRDY_Length(getRDY_Length() + p1->getCpuTime());
	}

}

char  EDF::getPtype()
{
	return Ptype;
}

void EDF:: ScheduleAlgo(int time) // add processes from the ready list to a new list to fix order  
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
		setRDY_Length(getRDY_Length() - temp->getCpuTime());
	}
	else if (getCurrRun())
	{
		getCurrRun()->execute(time);
		TotalBusyTime++;
		TotalIdleTime = time - TotalBusyTime;
		if (!getCurrRun()->getIOqueue().isEmpty())
		{

			if (getCurrRun()->getIOqueue().peek().getFirstItem() == time)
			{
				sc->RuntoBlk(getCurrRun());
				if (!EDFrdy.isEmpty()) //run empty and ready contains processes
				{
					Process* temp;
					EDFrdy.dequeue(temp);
					setCurrRun(temp);
				}
			}
		}
		else if (getCurrRun()->getisFinished()) 
		{
			getCurrRun()->setTerminationTime(time);
			getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
			TotalTRT += getCurrRun() ->getTurnaroundDuration();
			sc->Trm(getCurrRun());
			setCurrRun(nullptr);
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
