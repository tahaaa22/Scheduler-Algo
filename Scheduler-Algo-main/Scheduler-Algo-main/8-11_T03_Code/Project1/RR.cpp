#include "RR.h"
#include "Scheduler.h"
RR::RR(Scheduler* sch)
{
    Curtime = 0;
    settype('r');
    sc = sch;
	currenttime = 0;
}

void RR::RDYlength()
{
    setRDY_Length(0);
    if (RdyQueue.isEmpty())
    {
        setRDY_Length(0);
        return;
    }
    Node <Process*> * curQPtr = RdyQueue.getHead();
    while (curQPtr)
    {
        setRDY_Length(getRDY_Length() + curQPtr->getItem()->gettimeRemaining());
        curQPtr = curQPtr->getNext();
    }
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
		getCurrRun()->execute(timestep); //execute
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


//void RR::ScheduleAlgo(int timestep)
//{
//    //rdy queue msh empty w mafya4 current run bardo
//    if (!RdyQueue.isEmpty() && !getCurrRun())
//    {
//        Process* temp;
//        RdyQueue.dequeue(temp);
//        if (temp->getfirsttime() == 0)
//        {
//            temp->setResponseTime(timestep - temp->getArrivalTime());
//            temp->setfirsttime(1);
//        }
//        setCurrRun(temp);
//        if (getCurrRun()->getnumIO() != 0)
//        {
//            int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
//            getCurrRun()->setblktime(IO_req + timestep);
//        }
//            Curtime = 0;
//       // setRDY_Length(getRDY_Length() - temp->getCpuTime()); //bn2s cpu time el fy el run
//
//    }
//
//    else if (getCurrRun())  //run not empty
//    {
//        Process* temp = getCurrRun();
//        int t = temp->gettimeRemaining();
//
//        bool migrate = true;
//        while (migrate)
//        {
//            migrate = sc->migrationrtf(temp, RTF);
//            if (migrate == true)
//            {
//                if (!RdyQueue.isEmpty()) //run empty and ready contains processes
//                {
//                    Process* temp;
//                    RdyQueue.dequeue(temp);
//                    setCurrRun(temp);
//                    if (getCurrRun()->getnumIO() != 0)
//                    {
//                        int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
//                        getCurrRun()->setblktime(IO_req + timestep);
//                    }
//                }
//            }
//        }
//        if (Curtime == TS && t > 1) // a5r sec fy el ts
//        {
//            getCurrRun()->execute(timestep); // bt2alal timestep b 1
//            TotalBusyTime++;
//            TotalIdleTime = timestep - TotalBusyTime;
//            Curtime = 0;
//
//            // BACK TO RDY QUEUE 
//            if (t > 0) // process time slice bt3ha 5las 5ls 
//            {
//                RdyQueue.enqueue(temp);
//            }
//
//            // FROM RUN TO TERMINATION
//            if (t == 0)
//            {
//                temp->setTerminationTime(timestep);
//                temp->setTurnaroundDuration(temp->getTerminationTime() - temp->getArrivalTime());
//               /* TotalTRT += temp->getTurnaroundDuration();*/
//                sc->Trm(temp);
//                setCurrRun(NULL);
//                if (!RdyQueue.isEmpty()) //run empty and ready contains processes
//                {
//                    Process* temp;
//                    RdyQueue.dequeue(temp);
//                    setCurrRun(temp);
//                    if (getCurrRun()->getnumIO() != 0)
//                    {
//                        int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
//                        getCurrRun()->setblktime(IO_req + timestep);
//                    }
//
//                }
//            }
//            // FROM RUN TO BLOCK
//            if (getCurrRun()->getnumIO() != 0)
//            {
//                if (getCurrRun()->getblktime() == timestep)
//                {
//                    getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
//                    sc->RuntoBlk(temp);
//                    setCurrRun(NULL);
//                    if (!RdyQueue.isEmpty()) //run empty and ready contains processes
//                    {
//                        Process* temp;
//                        RdyQueue.dequeue(temp);
//                        setCurrRun(temp);
//                        if (getCurrRun()->getnumIO() != 0)
//                        {
//                            int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
//                            getCurrRun()->setblktime(IO_req + timestep);
//                        }
//
//                    }
//                }
//            }
//            
//
//        }
//        else
//        {
//            getCurrRun()->execute(timestep); //a2alal el timestep
//            TotalBusyTime++;
//            TotalIdleTime = timestep - TotalBusyTime;
//            Curtime++;
//            // ADDDDD COND BLOCK
//            if (getCurrRun()->getnumIO() != 0)
//            {
//
//                if (getCurrRun()->getblktime() == timestep)
//                {
//                    getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1);
//                    sc->RuntoBlk(getCurrRun());
//                    if (!RdyQueue.isEmpty()) //run empty and ready contains processes
//                    {
//                        Process* temp;
//                        RdyQueue.dequeue(temp);
//                        setCurrRun(temp);
//                        if (getCurrRun()->getnumIO() != 0)
//                        {
//                            int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
//                            getCurrRun()->setblktime(IO_req + timestep);
//                        }
//                    }
//                    Curtime = 0;
//                }
//            }
//            else if (getCurrRun()->getisFinished())
//            {
//                getCurrRun()->setTerminationTime(timestep);
//                getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
//               /* TotalTRT += getCurrRun()->getTurnaroundDuration();*/
//                sc->Trm(getCurrRun());
//                if (!RdyQueue.isEmpty()) //run empty and ready contains processes
//                {
//                    Process* temp;
//                    RdyQueue.dequeue(temp);
//                    setCurrRun(temp);
//                    if (getCurrRun()->getnumIO() != 0)
//                    {
//                        int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
//                        getCurrRun()->setblktime(IO_req + timestep);
//                    }
//                }
//                Curtime = 0;
//
//            }
//
//
//        }
//    }
//    RDYlength();
//}



void RR::print_rdy()
{
    RdyQueue.Print();

}
int RR::getRDYCount()
{
    return RdyQueue.getCount();
}

void RR::Loadp(ifstream& inputFile) {
    inputFile >> TS;
    inputFile >> RTF;

}
char RR::Ptype = 'r';

int RR::TS = 0;
int RR::RTF = 0;