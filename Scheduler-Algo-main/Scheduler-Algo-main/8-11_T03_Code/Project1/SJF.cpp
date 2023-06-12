#include "SJF.h"
#include "Scheduler.h"

SJF::SJF(Scheduler* sch)
{
    settype('s');
    sc = sch;
}

void SJF::OverheatRun(int OverheatConstant)
{
    isOverheated = true;
    if (sc->getMinProcessor(1, 0) == NULL && getCurrRun()) // if all processors are overheated all processes in the ready and the run will be terminated
        sc->Trm(getCurrRun());
    else  // there is a functioning ready list
        sc->AddtoRdy(getCurrRun());

    setCurrRun(nullptr);
    setOverheatTime(OverheatConstant);  // starts a timer for which the processor will be out of service
}

SJF::~SJF() {}

Process* SJF::gettop()
{
    Process* p;
    if (!PQ.isEmpty())  // checks if the ready list contains any processes 
    {
        PQ.dequeue(p);  // removes it from the ready list
        setRDY_Length(getRDY_Length() - p->gettimeRemaining());  // updates the ready list length
        return p;
    }
    else
        return NULL;  // returns nullptr if the ready list is empty
}

double SJF::pUtil()
{
    return (TotalBusyTime / (TotalBusyTime + TotalIdleTime));
}
void  SJF::addToReadyQueue(Process* p1, int time) //inserting a process to the RDY 
{
    if (p1)  // checks if the pointer sent is not a nullptr 
    {
        PQ.enqueue(p1, p1->gettimeRemaining());  // adds the process to the ready list
        setRDY_Length(getRDY_Length() + p1->gettimeRemaining());  // updates the ready list length
    }
}
char  SJF::getPtype()
{
    return Ptype;
}

Process* SJF::eject()
{
    Process* temp;
    PQ.dequeue(temp);  // removes it from the ready list
    setRDY_Length(getRDY_Length() - temp->gettimeRemaining());  // updates the ready list length
    return temp;
}


void SJF::Handle(int timestep) //this functions executes and checks if the process needs termination
{

    while (getCurrRun())  // while the is a process in the run state 
    {
        if (currenttime == timestep)  // prevents multiple incrementations of the busy time at same time step 
        {
            TotalBusyTime++;
            TotalIdleTime = timestep - TotalBusyTime;
        }
        currenttime++;
        if (getCurrRun()->getnumIO() > 0)  // checks if it will move to the blocked queue
        {
            if (getCurrRun()->getblktime() == timestep)
            {
                getCurrRun()->setTotalIO_R((getCurrRun()->getTotalIO_R()) + (getCurrRun()->getIOqueue()->peekR()->getFirstItem()));
                getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
                sc->RuntoBlk(getCurrRun());
                setCurrRun(nullptr);
                continue;
            }
        }
        if (getCurrRun()->getfirsttime() == 1)  // doesn't execute if it just entered the run
            getCurrRun()->setfirsttime(2);  // sets the firsttime to 2 to indicate the starting from next time it should execute 
        else if (getCurrRun()->getfirsttime() == 2)
            getCurrRun()->execute(timestep);  // executes the process in the run
        if (getCurrRun()->getisFinished())  // if the process in the run is finished 
        {
            getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());  // sets the turn around duration
            sc->Trm(getCurrRun());  // sends the process to the terminate list 
            setCurrRun(nullptr);  // sets the run to nullptr 
        }
        break;
    }
}

void SJF::ScheduleAlgo(int timestep)
{
    currenttime = timestep;
    if (isOverheated)
    {
        int t = getOverheatTime(); // processor 2
        setOverheatTime(getOverheatTime() - 1);
        if (getOverheatTime() == 0) { isOverheated = false; }
    }
    else
    {
        Handle(timestep); //equivalent to while run = true (run contains a process)
        while (!getCurrRun()) //while RUN is empty 
        {
            if (!PQ.isEmpty()) //run empty and ready contains processes
            {
                Process* temp; //First Process In is at the head, and the turn is on this Process to RUN
                PQ.dequeue(temp); //deleting first Process as it is removed to RUN
                setCurrRun(temp);
                setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy 
                if (temp->getfirsttime() == 0)
                {
                    temp->setResponseTime(timestep - temp->getArrivalTime());
                    temp->setfirsttime(1);
                }
                if (getCurrRun()->getnumIO() > 0 && (getCurrRun()->getIOqueue()->peekR()->getFirstItem() != 0))
                {
                    int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
                    getCurrRun()->setblktime(IO_req + timestep - (getCurrRun()->getTotalIO_R()));
                }
                Handle(timestep); //handles the current run
            }
            else
                break;
        }
    }

}



void  SJF::print_rdy()
{
    PQ.Print();
}


int  SJF::getRDYCount()
{
    return PQ.getCount();
}


char SJF::Ptype = 's';