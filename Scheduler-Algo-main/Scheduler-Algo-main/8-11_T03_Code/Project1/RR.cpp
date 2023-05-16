#include "RR.h"
#include "Scheduler.h"


RR::RR(Scheduler* sch)
{
    Curtime = 0;
    settype('r');
    sc = sch;
	currenttime = 0;
}

Process* RR::gettop()
{
    Process* p = NULL;
    RdyQueue.peek(p);
    if (p && (p->getorphanflag()))
        return NULL;
    else
    {
		if (p)
		{
			RdyQueue.dequeue(p);
			setRDY_Length(getRDY_Length() - p->gettimeRemaining());
			return p;
		}
		else
			return NULL;
    }
}


double RR::pUtil()
{
    return (TotalBusyTime / (TotalBusyTime + TotalIdleTime));
}
char RR::getPtype()
{
    return Ptype;
}



Process* RR::eject()
{
    Process* temp;
    if (!RdyQueue.isEmpty())
    {
        RdyQueue.dequeue(temp);
        setRDY_Length(getRDY_Length() - temp->gettimeRemaining());
        return temp;
    }
    return nullptr;
}

void RR::addToReadyQueue(Process* p1)
{
    if (p1)
    {
        RdyQueue.enqueue(p1);
		setRDY_Length(getRDY_Length() + p1->gettimeRemaining());; // lw tel3et 3ala fady ya taha shelha wenta btnadaf
    }
}

void RR::Handle(int timestep) //this functions executes and checks if the process needs termination
{
	
	while (getCurrRun())
	{
		if (currenttime == timestep)
		{
			TotalBusyTime++;
			TotalIdleTime = timestep - TotalBusyTime;
		}
		currenttime++;
		////////////////////////////RUNNING PROCESS HAS NO ACTION TO DO IN THIS TIME STEP///////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		if (getCurrRun()->getfirsttime() == 1)
			getCurrRun()->setfirsttime(2);
		else if (getCurrRun()->getfirsttime() == 2)
			getCurrRun()->execute(timestep); //execute
		////////////////////////////////////////////////////////////////////////////
		Curtime++; // to know time slice 
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		// HANDLING MIGRATION //
		bool migrate = sc->migrationrtf(getCurrRun(), RTF);
	
			if (migrate == true) //&& RDY.getHead()
			{
				//Process* temp = RDY.getHead()->getItem();
				//RDY.deleteNode();
				//setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
				//setCurrRun(temp);
				//migrate = sc->migrationmaxw(getCurrRun(), MaxW, timestep);
				setCurrRun(NULL);
				continue;
			}
		
		//handling blk//
		if (getCurrRun()->getnumIO() > 0)
		{
			if (getCurrRun()->getblktime() == timestep)
			{
				getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
				sc->RuntoBlk(getCurrRun());
				//setRDY_Length(getRDY_Length() - getCurrRun()->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
				setCurrRun(nullptr);
				Curtime = 0;
				continue;
			}
		}

		//HANDLING TERMINATION //
		if (getCurrRun()->getisFinished())
		{
			if (getCurrRun()->getPID() == 11)
				int t = 0;
			//getCurrRun()->setTerminationTime(timestep);
			getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
			//TotalTRT += getCurrRun()->getTurnaroundDuration(); // waiting for DR to remove 
			sc->Trm(getCurrRun());
			//setRDY_Length(getRDY_Length() - getCurrRun()->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
			setCurrRun(nullptr);
			Curtime = 0;
		}

		//HANDLING TIME SLICE IF PROCESS WILL CONTINUE IN THE RUNNING STATE//
		if (Curtime == TS)
		{
			RdyQueue.enqueue(getCurrRun());
			setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
			setCurrRun(NULL);
			Curtime = 0;
		}

		break;
	}
}

void RR::ScheduleAlgo(int timestep)
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
		if (getisOverheated())
		{
			int t = getOverheatTime(); //processor 3
			setOverheatTime(getOverheatTime() - 1);
			if (getOverheatTime() == 0) setisOverheated(false);
		}
		else
		{
			Handle(timestep); //equivalent to while run = true (run contains a process)
			while (!getCurrRun()) //while RUN is empty 
			{
				if (!RdyQueue.isEmpty()) //run empty and ready contains processes
				{
					Process* temp; //First Process In is at the head, and the turn is on this Process to RUN 
					RdyQueue.dequeue(temp); //deleting first Process as it is removed to RUN
					setCurrRun(temp);
					setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
					if (temp->getfirsttime() == 0)
					{
						temp->setResponseTime(timestep - temp->getArrivalTime());
						temp->setfirsttime(1);
					}
					setCurrRun(temp);
					if (getCurrRun()->getnumIO() != 0) // check to go to blk 
					{
						int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
						getCurrRun()->setblktime(IO_req + timestep);
					}
					Handle(timestep); //handles the new current run 
				}
				else
					break;
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

void RR::Loadp(ifstream& inputFile) 
{
    inputFile >> TS;
    inputFile >> RTF;

}

RR::~RR() {}

char RR::Ptype = 'r';
int RR::TS = 0;
int RR::RTF = 0;