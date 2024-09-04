#include "FCFS.h"
#include "Scheduler.h"
using namespace std;

FCFS::FCFS(Scheduler* sch)
{
	settype('f'); //type of processor fcfs = 'f'
	sc = sch; //a pointer of scheduler
}

void FCFS::OverheatRun(int OverheatConstant) //used in overheating if process is in RUN
{
	setisOverheated(true);
	if (!sc->getMinProcessor(1, 0) && getCurrRun()) // if all processors are overheated all processes in the ready and the run will be terminated
		sc->Trm(getCurrRun()); //process in RUN sent to Termination
	else
		sc->AddtoRdy(getCurrRun()); //process in RUN sent to shortest RDY queue

	setCurrRun(nullptr);
	setOverheatTime(OverheatConstant);
}

void FCFS::deleteNodeK(Process* p) //used in killing from the middle of rdy
{
	RDY.deleteNode(p);
	setRDY_Length(getRDY_Length() - p->gettimeRemaining());
}

Process* FCFS::eject()  //acts as a dequeue
{
	Process* temp = RDY.peek();
	deleteNodeK(temp);
	return temp;
}

Process* FCFS::gettop()
{
	if (RDY.getHead())
	{
		Process* p = RDY.peek(); // peeking process at the top due to stealing 
		if (p && (p->getorphanflag())) //if the process is forked
			return NULL; //do not steal it as forked processes can only exist in FCFS processors
		else
		{
			if (p)
			{
				deleteNodeK(p);
				return p;
			}
		}
	}
	return nullptr;
}


bool FCFS::isfound(Process* p) //if process is found in this RDY list
{
	return (RDY.isFound(p));
}

double FCFS::pUtil() //calculating %utilization
{
	return (TotalBusyTime / (TotalBusyTime + TotalIdleTime));
}

int FCFS::getRDYCount() //number of processes in the ready list 
{
	return RDY.getCount();
}

void FCFS::KillRun(Process* orphan) //kills a process in the RUN state
{
	if (getCurrRun() == orphan)
	{
		cout << endl;
		cout << "orphan " << orphan <<  " from run terminated\n" << endl;
		sc->Trm(orphan);
		setCurrRun(nullptr);
	}
	return;
}

void FCFS::KillOrphan(Process* subRoot, int time) //killing children and grandchildren of a subRoot
{
	if ((!subRoot->getLCH() && !subRoot->getRCH()) || !subRoot) //no children or no parent
	{
		return; //base case
	}
	//Killing grandchildren then children
	if (subRoot->getLCH())
	{
		KillOrphan(subRoot->getLCH(), time);
		subRoot->getLCH()->setwaitingtime(subRoot->getLCH()->gettimeRemaining());
		subRoot->getLCH()->setTerminationTime(time); 
		subRoot->getLCH()->setTurnaroundDuration(subRoot->getLCH()->getTerminationTime() - subRoot->getLCH()->getArrivalTime());
		sc->Kill(subRoot->getLCH());
		subRoot->setLCH(nullptr);
	}
	if (subRoot->getRCH())
	{
		KillOrphan(subRoot->getRCH(), time);
		subRoot->getRCH()->setwaitingtime(subRoot->getRCH()->gettimeRemaining());
		subRoot->getRCH()->setTerminationTime(time);
		subRoot->getRCH()->setTurnaroundDuration(subRoot->getRCH()->getTerminationTime() - subRoot->getRCH()->getArrivalTime());
		sc->Kill(subRoot->getRCH());
		subRoot->setRCH(nullptr);
	}
	return;
}

bool FCFS::sigkill(int timestep, int NF)
{
	SNode<int> Q; //struct containing the ID and timestep
	if (killSig.isEmpty())
		return false;
	Q = *(killSig.peek());
	int kill = Q.getFirstItem(); // get the kill time
	int ID = Q.getSecondItem(); // get id to be killed
	Node<Process*>* temp = RDY.getHead(); // get head
	if (timestep == kill)
	{
		nf++; // add by 1 as we traversed another fcfs processor
		//check if process is in this processor FCFS[nf] RDY
		while (temp) //traversing the RDY list until finding the process with that ID
		{
			
			if (temp->getItem()->getPID() == ID) //finding the process to be killed 
			{
				sc->setpKill(sc->getpKill() + 1);
				Process* p = temp->getItem();
				KillOrphan(p, timestep); // killing the children and grandchildren and decrementing rdy length
				p->setiskilled(true);
				p->setwaitingtime(p->gettimeRemaining());
				setRDY_Length(getRDY_Length() - p->gettimeRemaining()); //adjusting RDY_Length
				if (p->getorphanflag()) //if the process is forked
				{
					p->setwaitingtime(p->gettimeRemaining());
					//check whether it is a right child or left child
					if (p->getParent()->getLCH() == p) //if it is a left child for a parent
					{
						p->getParent()->setLCH(nullptr);
					}
					if (p->getParent()->getRCH() == p) //if it is a right child for a parent
					{
						p->getParent()->setRCH(nullptr);
					}
				}
				RDY.deleteNode(p); //delete it from RDY List
				p->setTerminationTime(timestep);
				p->setTurnaroundDuration(p->getTerminationTime() - p->getArrivalTime());
				sc->Trm(p); //terminate this process
				break;
			}
			temp = temp->getNext();
		}
		if (getCurrRun()) //check if process is in RUN
		{
			
			int IDR = getCurrRun()->getPID();
			if (IDR == ID)
			{
				getCurrRun()->setiskilled(true);
				getCurrRun()->setwaitingtime(getCurrRun()->gettimeRemaining());
				sc->setpKill(sc->getpKill() + 1);
				KillOrphan(getCurrRun(), timestep); // killing the children and grandchildren
				getCurrRun()->setTerminationTime(timestep);
				getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
				sc->Trm(getCurrRun()); //terminate this process
				setCurrRun(nullptr);// RUN CURRENTLY EMPTY
			}

		}

		if (nf == NF) //traversed all fcfs 
		{
			killSig.dequeue(); // dequeue the element we traversed all processors
			if (killSig.isEmpty()) //if empty return 
				return false;
			Q = *(killSig.peek());
			kill = Q.getFirstItem(); // get the kill time
			ID = Q.getSecondItem(); // get id to be killed
			temp = RDY.getHead(); // get head
			nf = 0;
			if (kill == timestep)
				return true; // want to kill again so send true
		}
	}
	else
		return false;
}


void FCFS::Handle(int timestep) //this functions executes and checks if the process needs termination
{
	while (getCurrRun())
	{
		
		if (currenttime == timestep)
		{
			TotalBusyTime++;
			TotalIdleTime = timestep - TotalBusyTime;
		}
		currenttime++;

		//handling fork
		srand(timestep);
		double forkP = rand() % 100; //generate a random forking probability
		if (forkP <= forkprob && forkprob != 100) //compare the randomly generate to the one from the input file
		{
			sc->fork(getCurrRun());
		}

		bool migrate = sc->migrationmaxw(getCurrRun(), MaxW, timestep);

		if (migrate == true)
		{
			
			setCurrRun(NULL);
			continue;
		}
		//handling blk
		if (getCurrRun()->getnumIO() > 0) //the process has IO
		{
			if (getCurrRun()->getblktime() == timestep)
			{
				getCurrRun()->setTotalIO_R((getCurrRun()->getTotalIO_R()) + (getCurrRun()->getIOqueue()->peekR()->getFirstItem()));
				getCurrRun()->setnumIO(getCurrRun()->getnumIO() - 1); //decrement the remaining IO_Pairs
				sc->RuntoBlk(getCurrRun()); //send to blk
				setCurrRun(nullptr); //free the RUN 
				continue;
			}
		}


		if (getCurrRun()->getfirsttime() == 1)  //process is just added to the RDY queue at this timestep
			getCurrRun()->setfirsttime(2); // 2 is the flag that means we can now execute the process (to be used in next timestep)
		else if (getCurrRun()->getfirsttime() == 2)
		{
			getCurrRun()->execute(timestep); //execute
		}

		if (getCurrRun()->getisFinished()) //the process finished execution and needs to be terminated
		{
			if (getCurrRun()->getorphanflag() == true)
			{
				KillOrphan(getCurrRun(), timestep); //kill its children before sending it to termination
			}
			getCurrRun()->setTerminationTime(timestep);
			getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
			// to make sure waiting time of killed orphans not with negative value
			getCurrRun()->setwaitingtime(getCurrRun()->getTurnaroundDuration() - getCurrRun()->getCpuTime());
			sc->Trm(getCurrRun());
			setCurrRun(nullptr);
		}
		break;
	}
}


void FCFS::ScheduleAlgo(int timestep)
{
	currenttime = timestep;
	if (getisOverheated())
	{
		setOverheatTime(getOverheatTime() - 1);
		if (getOverheatTime() == 0) setisOverheated(false);
	}
	else
	{
		Handle(timestep); //equivalent to while run = true (run contains a process)
		while (!getCurrRun()) //while RUN is empty 
		{
			if (!RDY.isEmpty()) //run empty and ready contains processes
			{

				Process* temp = RDY.peek(); //First Process In is at the head, and the turn is on this Process to RUN
				setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
				RDY.deleteNode(); //deleting first Process as it is removed to RUN
				if (temp->getfirsttime() == 0)
				{
					temp->setResponseTime(timestep - temp->getArrivalTime());
					temp->setfirsttime(1);

				}
				setCurrRun(temp);
				int t = getCurrRun()->getnumIO();
				if (getCurrRun()->getnumIO() > 0 )
				{
					int t = getCurrRun()->getnumIO();
					int IO_req = getCurrRun()->getIOqueue()->peekR()->getFirstItem();
					getCurrRun()->setblktime(IO_req + timestep - (getCurrRun()->getTotalIO_R()));
					//getCurrRun()->getIOqueue()->peekR()->setFirstItem((getCurrRun()->getIOqueue()->peekR()->getFirstItem()) - (getCurrRun()->getTotalIO_R())); // make summation
				}
				Handle(timestep); //handles the current run
			}
			else
				break;
		}

	}
}


void FCFS::addToReadyQueue(Process* process, int time) //inserting a process to the RDY 
{
	if (process)
	{
		RDY.insertNode(process);
		setRDY_Length(getRDY_Length() + process->gettimeRemaining()); //adjusting rdy length
	}
}

void FCFS::print_rdy()
{
	RDY.Print();
}

char FCFS::getPtype()
{
	return Ptype;
}

//loading parameters related to FCFS

void FCFS::Loadp(ifstream& inputFile)
{
	inputFile >> MaxW;
}

void FCFS::Loadpf(ifstream& inputFile)
{
	inputFile >> forkprob;
}

void FCFS::Loadkill(ifstream& inputFile)
{
	while (inputFile.peek() != EOF)
	{
		int time;
		int pid;
		inputFile >> time;
		inputFile >> pid;
		SNode<int>* kill = new SNode <int>(time, pid);
		killSig.enqueue(kill);
	}
}

FCFS :: ~FCFS() {} //destructor

SQueue<int> FCFS::killSig; //kill signal queue

char FCFS::Ptype = 'f';
double FCFS::forkprob = 0;
int FCFS::MaxW = 0;
int FCFS::nf = 0; // no of fcfs processor that i traversed