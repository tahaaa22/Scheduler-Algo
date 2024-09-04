#include "EDF.h"
#include "Scheduler.h"

EDF::EDF(Scheduler* sch)
{
    settype('e');
    sc = sch;
}
void EDF::OverheatRun(int OverheatConstant)
{
    isOverheated=true;
    if (sc->getMinProcessor(1, 0) == NULL && getCurrRun()) // if all processors are overheated all processes in the ready and the run will be terminated
        sc->Trm(getCurrRun());
    else  // there is a functioning ready list
        sc->AddtoRdy(getCurrRun());

    setCurrRun(nullptr);
    setOverheatTime(OverheatConstant);  // starts a timer for which the processor will be out of service
}
Process* EDF::gettop()
{
    Process* p;
    EDFrdy.peek(p);
    if (p)  // checks if the ready list contains any processes 
    {
        EDFrdy.dequeue(p);  // removes it from the ready list
        setRDY_Length(getRDY_Length() - p->gettimeRemaining());  // updates the ready list length
        return p;
    }
    else
        return NULL;  // returns nullptr if the ready list is empty
}

double EDF::pUtil()
{
    return (TotalBusyTime / (TotalBusyTime + TotalIdleTime));
}

void  EDF::addToReadyQueue(Process* p1, int time) //inserting a process to the RDY 
{
    if (p1)  // checks if the pointer sent is not a nullptr 
    {
        if (!getCurrRun())  // if no process in the run state 
        {
            setRDY_Length(getRDY_Length() + p1->gettimeRemaining());  // updates the ready length
            EDFrdy.enqueue(p1, p1->getDeadLine());  // adds it to the ready list 

        }
        else  // if there is a process in the run 
        {
            if (p1->getDeadLine() < getCurrRun()->getDeadLine())  // if the new process' has an earlier deadline than the one in the run
            {
                setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining());  // returns the process in the run to the ready list and updates the ready length
                EDFrdy.enqueue(getCurrRun(), getCurrRun()->getDeadLine());
                setCurrRun(p1);  // adds the new process to the run state  
                if (getCurrRun()->getfirsttime() == 0)  // checks if this is the process' first time in any run state
                {
                    getCurrRun()->setResponseTime(time - getCurrRun()->getArrivalTime());  // sets the arrival time 
                    getCurrRun()->setfirsttime(1);  // indicates it just enters the run
                }
                if (getCurrRun()->getnumIO() > 0 && (getCurrRun()->getIOqueue()->peekR()->getFirstItem() != 0))  // checks if the process will request I/O
                {
                    int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
                    getCurrRun()->setblktime(IO_req + time - (getCurrRun()->getTotalIO_R()));  // sets the time at which it will move to the blocked queue 

                }
            }
            else  // if the new process' has a latter deadline than the one in the run
            {
                setRDY_Length(getRDY_Length() + p1->gettimeRemaining());  // adds the new process to the ready list and updates the ready length
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
    EDFrdy.dequeue(temp);  // removes it from the ready list
    setRDY_Length(getRDY_Length() - temp->gettimeRemaining());  // updates the ready list length
    return temp;
}

void EDF::Handle(int timestep) //this functions executes and checks if the process needs termination
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
            getCurrRun()->setfirsttime(2);  // sets the first time to 2 to indicate the starting from next time it should execute 
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


void EDF::ScheduleAlgo(int timestep)
{
    currenttime = timestep;
    if (isOverheated)  // checks if the processor is overheated 
    {
        setOverheatTime(getOverheatTime() - 1);  // decrements the overheat time left 
        if (getOverheatTime() == 0)  // if the overheat time left is finished
            isOverheated=false;  // returns the processor in operation
    }
    else
    {
        Handle(timestep); // handles the process in the run state 
        while (!getCurrRun()) //while RUN is empty 
        {
            if (!EDFrdy.isEmpty()) //run empty and ready contains processes
            {
                Process* temp; //First Process In is at the head, and the turn is on this Process to RUN
                EDFrdy.dequeue(temp); //deleting first Process as it is removed to RUN
                setCurrRun(temp);
                setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy 
                if (temp->getfirsttime() == 0)  // checks if this is the process' first time in any run state
                {
                    temp->setResponseTime(timestep - temp->getArrivalTime());  // sets the arrival time 
                    temp->setfirsttime(1);  // indicates it just enters the run
                }
                if (getCurrRun()->getnumIO() > 0 && (getCurrRun()->getIOqueue()->peekR()->getFirstItem() != 0))  // checks if the process will request I/O
                {
                    int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
                    getCurrRun()->setblktime(IO_req + timestep - (getCurrRun()->getTotalIO_R()));  // sets the time at which it will move to the blocked queue 
                    
                }
                Handle(timestep);  // handles the process in the run
            }
            else
            {
                break;
            }
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

EDF::~EDF() {}