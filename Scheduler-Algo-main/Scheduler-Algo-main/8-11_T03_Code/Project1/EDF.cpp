#include "EDF.h"
#include "Scheduler.h"

EDF::EDF()
{
	settype('e');
}

void  EDF::addToReadyQueue(Process* p1) //inserting a process to the RDY 
{
	if(p1->getDeadLine() < getCurrRun()->getDeadLine())
	{
		EDFrdy.enqueue(getCurrRun(), getCurrRun()->getDeadLine());
		setCurrRun(p1);
		setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining());
	}
	EDFrdy.enqueue(p1, p1->getDeadLine());
	setRDY_Length(getRDY_Length() + p1->getCpuTime());
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
		setCurrRun(temp);
		setRDY_Length(getRDY_Length() - temp->getCpuTime());
	}
	else if (getCurrRun())
	{
		getCurrRun()->execute(time);
		if (!getCurrRun()->getIOqueue().isEmpty())
		{

			if (getCurrRun()->getIOqueue().peek().getFirstItem() == time)
			{
				sc->RuntoBlk(getCurrRun());
				setCurrRun(nullptr);
			}
		}
		else if (getCurrRun()->getisFinished()) 
		{
			sc->RuntoTrm(getCurrRun());
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