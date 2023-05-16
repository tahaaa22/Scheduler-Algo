#include "SJF.h"
#include "Scheduler.h"

SJF::SJF(Scheduler * sch) 
{
    settype('s');
    sc = sch;
}

SJF::~SJF() {}

Process* SJF::gettop()
{
    Process* p= NULL ;
    PQ.peek(p);
    if (p && (p ->getorphanflag()))
    return NULL ;
    else
    {
        if (p)
        {
            PQ.dequeue(p);
            setRDY_Length(getRDY_Length() - p->gettimeRemaining());
            return p;
        }
        else
            return NULL;
    }
 }

double SJF::pUtil()
{
    return (TotalBusyTime / (TotalBusyTime + TotalIdleTime)) ;
}
void  SJF::addToReadyQueue(Process* p1) //inserting a process to the RDY 
{
    if (p1)
    {
        PQ.enqueue(p1, p1->gettimeRemaining());
        setRDY_Length(getRDY_Length() + p1->gettimeRemaining());
    }
}
char  SJF::getPtype()
{
    return Ptype;
}

Process* SJF::eject()   
{
    Process* temp;
    PQ.dequeue(temp);
    setRDY_Length(getRDY_Length() - temp->gettimeRemaining());
    return temp;
}


void SJF::Handle(int timestep) //this functions executes and checks if the process needs termination
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
          //  getCurrRun()->setTerminationTime(timestep);
            getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
            //TotalTRT += getCurrRun()->getTurnaroundDuration(); // waiting for DR to remove 
            sc->Trm(getCurrRun());
            setCurrRun(nullptr);
        }

        break;
    }
}

void SJF::ScheduleAlgo(int timestep)
{
        currenttime = timestep;
        if (getisOverheated())
        {
            int t = getOverheatTime(); // processor 2
            setOverheatTime(getOverheatTime() - 1);
            if (getOverheatTime() == 0) setisOverheated(false);
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
                    if (getCurrRun()->getnumIO() != 0)
                    {
                        int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
                        getCurrRun()->setblktime(IO_req + timestep);
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

void  SJF::Loadp(ifstream& inputFile)
{
    return;
}

int  SJF::getRDYCount()
{
    return PQ.getCount();
}


char SJF::Ptype = 's';
