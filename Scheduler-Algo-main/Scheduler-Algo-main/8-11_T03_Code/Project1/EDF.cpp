#include "EDF.h"
#include "Scheduler.h"

EDF :: EDF(Scheduler * sch)
{
	settype('e');
	sc = sch;
}
Process* EDF::gettop()
{
	Process* p = NULL ;
	EDFrdy.peek(p);
	if (p&&(p->getorphanflag()))
		return NULL;
	else
	{
        if (p)
        {
            EDFrdy.dequeue(p);
            if (getRDY_Length() < 0)
                int t = 0;
            setRDY_Length(getRDY_Length() - p->gettimeRemaining());
            return p;
        }
        else
            return NULL;
	}
}
 
double EDF::pUtil()
{
	return (TotalBusyTime / (TotalBusyTime + TotalIdleTime)) ;
}
void  EDF::addToReadyQueue(Process* p1) //inserting a process to the RDY 
{
    if (getRDY_Length() < 0)
        int t = 0;
  
    if (p1)
    {
        if (!getCurrRun())
        {
            setRDY_Length(getRDY_Length() + p1->gettimeRemaining());
            EDFrdy.enqueue(p1, p1->getDeadLine());
            
        }
        else 
        {
            if (p1->getDeadLine() < getCurrRun()->getDeadLine())
            {
                setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining());
                EDFrdy.enqueue(getCurrRun(), getCurrRun()->getDeadLine());
                setCurrRun(p1);
                if (getCurrRun()->getnumIO() != 0)
                {
                    int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
                    getCurrRun()->setblktime(IO_req + sc->getTimeStep());
                }
            }
            else
            {
                setRDY_Length(getRDY_Length() + p1->gettimeRemaining());
                EDFrdy.enqueue(p1, p1->getDeadLine());
            }
        }
       
    }

}

char  EDF::getPtype()
{
	return Ptype;
}

Process* EDF::eject()
{
    Process* temp;
    EDFrdy.dequeue(temp);
    setRDY_Length(getRDY_Length() - temp->gettimeRemaining());
    return temp;
}



void EDF::Handle(int timestep) //this functions executes and checks if the process needs termination
{

    while (getCurrRun())
    {
        if (currenttime == timestep)
        {
            TotalBusyTime++;
            TotalIdleTime = timestep - TotalBusyTime;
        }
        currenttime++;

        //handling blk//
        if (getCurrRun()->getnumIO() > 0)
        {
            if (getCurrRun()->getblktime() == timestep)
            {
                getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
                sc->RuntoBlk(getCurrRun());
                setCurrRun(nullptr);
                continue;
            }
        }
  
       ///////////////////////////////////////////////////////////////////////////
        if (getCurrRun()->getfirsttime() == 1)
            getCurrRun()->setfirsttime(2);
        else if (getCurrRun()->getfirsttime() == 2)
            getCurrRun()->execute(timestep); //execute
        ////////////////////////////////////////////////////////////////////////////
        if (getCurrRun()->getisFinished())
        {
            if (getCurrRun()->getPID() == 11)
                int t = 0;
           // getCurrRun()->setTerminationTime(timestep);
            getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
            //TotalTRT += getCurrRun()->getTurnaroundDuration(); // waiting for DR to remove 
            sc->Trm(getCurrRun());
            setCurrRun(nullptr);
        }

        break;
    }
}

void EDF::ScheduleAlgo(int timestep)
{
    currenttime = timestep;
    if (getisOverheated())
    {
        int t = getOverheatTime(); // processor 1
        setOverheatTime(getOverheatTime() - 1);
        if (getOverheatTime() == 0) setisOverheated(false);
    }
    else
    {
        Handle(timestep); //equivalent to while run = true (run contains a process)
        while (!getCurrRun()) //while RUN is empty 
        {
            if (!EDFrdy.isEmpty()) //run empty and ready contains processes
            {
                Process* temp; //First Process In is at the head, and the turn is on this Process to RUN
                EDFrdy.dequeue(temp); //deleting first Process as it is removed to RUN
                setCurrRun(temp);
                if (getRDY_Length() < 0)
                    int t = 0;
                setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy 
                if (temp->getfirsttime() == 0)
                {
                    temp->setResponseTime(timestep - temp->getArrivalTime());
                    temp->setfirsttime(1);
                }
                if (getCurrRun()->getnumIO() != 0)
                {
                    int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
                    getCurrRun()->setblktime(IO_req + timestep);
                }
                Handle(timestep); //handles the current run
            }
            else
            {
                break;
            }
        }
    }
}
//void EDF :: RDYlength()
//{
//    setRDY_Length(0);
//    if (EDFrdy.isEmpty())
//    {
//        setRDY_Length(0);
//        return;
//    }
//    PQNode <Process*>* curQPtr = EDFrdy.getHead();
//    while (curQPtr)
//    {
//        setRDY_Length(getRDY_Length() + curQPtr->getItem()->gettimeRemaining());
//        curQPtr = curQPtr->getNext();
//    }
//}

void  EDF::print_rdy()
{
	EDFrdy.Print();
}

int  EDF::getRDYCount()
{
	return EDFrdy.getCount();
}
char EDF::Ptype = 'e';

EDF::~EDF() {}
