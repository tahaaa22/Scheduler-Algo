#include "SJF.h"
#include "Scheduler.h"
SJF::SJF() {
    settype('s');
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

//Process* SJF::sigkill(int timestep, int NF)
//{
//    return nullptr;
//}


void  SJF::ScheduleAlgo(int time)
{
    if (!PQ.isEmpty() && !getCurrRun()) {
        Process* temp;
        PQ.dequeue(temp);
        setCurrRun(temp);
        setRDY_Length(getRDY_Length() - temp->getCpuTime());
    }
    else if (getCurrRun())
    {
        getCurrRun()->execute(time);
        if (!getCurrRun()->getIOqueue().isEmpty()) {

            if (getCurrRun()->getIOqueue().peek().getFirstItem() == time) {
                sc->RuntoBlk(getCurrRun());
                setCurrRun(nullptr);
            }
        }
        else if (getCurrRun()->getisFinished()) {
            sc->RuntoTrm(getCurrRun());
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
