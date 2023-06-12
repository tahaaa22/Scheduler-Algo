#include "RR.h"
#include "Scheduler.h"


RR::RR(Scheduler* sch)
{
	Curtime = 0; // nothing is currently running
	settype('r'); // type RR
	sc = sch;
	currenttime = 0;
}

Process* RR::gettop()
{
	return eject();
}

char RR::getPtype()
{
	return Ptype; // return processor type
}

int RR::getRDYCount()
{
	return RdyQueue.getCount(); // get count
}

void RR::OverheatRun(int OverheatConstant)
{
	isOverheated=true; //Processor is overheated
	if (sc->getMinProcessor(1, 0) == NULL && getCurrRun()) // if all processors are overheated all processes in the ready and the run will be terminated
		sc->Trm(getCurrRun()); //Trm all the process
	else
		sc->AddtoRdy(getCurrRun()); // add to shortest ready

	setCurrRun(nullptr);
	setOverheatTime(OverheatConstant);
}

Process* RR::eject()
{
	Process* temp;
	if (!RdyQueue.isEmpty()) // empty the rdyqueue
	{
		RdyQueue.dequeue(temp);
		setRDY_Length(getRDY_Length() - temp->gettimeRemaining());
		return temp;
	}
	return nullptr;
}

void RR::ScheduleAlgo(int timestep)
{
	currenttime = timestep;
	if (isOverheated)
	{
		setOverheatTime(getOverheatTime() - 1);
		if (getOverheatTime() == 0) { isOverheated = false; }
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
				if (getCurrRun()->getnumIO() > 0 && (getCurrRun()->getIOqueue()->peekR()->getFirstItem() != 0)) // check to go to blk 
				{
					int t = getCurrRun()->getnumIO();
					int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
					getCurrRun()->setblktime(IO_req + timestep - (getCurrRun()->getTotalIO_R()));
				}
				Handle(timestep); //handles the new current run 
			}
			else
				break;
		}
	}
}

void RR::Handle(int timestep) //this functions executes and checks if the process needs termination
{
	// if run is currently not null
	while (getCurrRun())
	{
		if (currenttime == timestep)
		{
			TotalBusyTime++; // add it's busy time as it's running
			TotalIdleTime = timestep - TotalBusyTime;
		}
		currenttime++;
		////////////////////////////RUNNING PROCESS HAS NO ACTION TO DO IN THIS TIME STEP///////////////////////////////////////////////

		if (getCurrRun()->getfirsttime() == 1)
			getCurrRun()->setfirsttime(2); //if it's just arrived run don't execute it
		else if (getCurrRun()->getfirsttime() == 2)
		{
			getCurrRun()->execute(timestep); //execute
			Curtime++; // the process runned one timestep used to check if it's TS is finished
		}


		// HANDLING MIGRATION //
		bool migrate = sc->migrationrtf(getCurrRun(), RTF);

		if (migrate == true) 
		{
			setCurrRun(NULL); // set run now with null as this process already migrated
			continue;
		}

		//handling blk//
		if (getCurrRun()->getnumIO() > 0)
		{
			if (getCurrRun()->getblktime() == timestep) //check if process should go to blk
			{
				getCurrRun()->setTotalIO_R((getCurrRun()->getTotalIO_R()) + (getCurrRun()->getIOqueue()->peekR()->getFirstItem()));
				getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
				sc->RuntoBlk(getCurrRun()); // send the process to blk
				setCurrRun(nullptr); // set it to null
				Curtime = 0;
				continue;
			}
		}

		//HANDLING TERMINATION //
		if (getCurrRun()->getisFinished())
		{
			getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
			sc->Trm(getCurrRun());
			setCurrRun(nullptr);
			Curtime = 0; // set it back to zero for the new process
		}

		//HANDLING TIME SLICE IF PROCESS WILL CONTINUE IN THE RUNNING STATE//
		if (Curtime == TS)
		{
			RdyQueue.enqueue(getCurrRun()); // add the process back to rdyqueue
			setRDY_Length(getRDY_Length() + getCurrRun()->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
			setCurrRun(NULL);
			Curtime = 0; // set it back to zero for the new process
		}

		break;
	}
}

double RR::pUtil()
{
	return (TotalBusyTime / (TotalBusyTime + TotalIdleTime)); //calulate utilization
}

void RR::addToReadyQueue(Process* p1, int time)
{
	//check p is not null
	if (p1)
	{
		RdyQueue.enqueue(p1); //add to the queue
		setRDY_Length(getRDY_Length() + p1->gettimeRemaining());; // lw tel3et 3ala fady ya taha shelha wenta btnadaf
	}
}


void RR::print_rdy()
{
	RdyQueue.Print(); //print rdy

}

void RR::Loadp(ifstream& inputFile)
{
	inputFile >> TS; // load ts
	inputFile >> RTF; //load rtf

}

RR::~RR() {}

//////////////////////////////////static data members as all processor should see them/////////
char RR::Ptype = 'r';
int RR::TS = 0; // intilize it with 0 at first
int RR::RTF = 0; // intilize it with 0 at first