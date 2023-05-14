#include "FCFS.h"
#include "Scheduler.h"
using namespace std;

FCFS::FCFS(Scheduler * sch)
{
	settype('f');
	sc = sch;
}
//int FCFS::getRDY_Length()
//{
//	Node<Process*> * temp = RDY.getHead();
//	int sum = 0;
//	while (temp)
//	{
//		sum += temp->getItem()->gettimeRemaining();
//		temp = temp->getNext();
//	}
//	return sum;
//}

void FCFS::deleteNodeK(Process* p) //used for optimisation later
{
	RDY.deleteNode(p);
	setRDY_Length(getRDY_Length() - p->gettimeRemaining());
}

bool FCFS::isfound(Process* p)
{
	return (RDY.isFound(p));
}

Process* FCFS::gettop()
{
		if (RDY.getHead())
		{
			Process* p = RDY.peek(); // peeking process at the top due to stealing 
			if (p && (p->getorphanflag()))
				return NULL ;
			else
		    {
				if (p)
				{
					RDY.deleteNode(p);
					setRDY_Length(getRDY_Length() - p->gettimeRemaining());
					return p;
				}
			}
		}
	return nullptr;
}

double FCFS::pUtil()
{
	return (TotalBusyTime / (TotalBusyTime + TotalIdleTime));
}

int FCFS::getRDYCount()
{
	return RDY.getCount();
}

void FCFS::KillOrphan(Process* parent, int time)
{
	//if (parent->getPID() > 0 && parent ->getPID() =< sNumProcess)
	if ((!parent->getLCH() && !parent->getRCH()) || !parent) //no children or no parent
	{
		return;
	}
	if (parent->getLCH())
	{
		KillOrphan(parent->getLCH(), time); //the recursive calls will stop when the subroot has LCH = NULL
		sc->Trm(parent->getLCH());

		parent->getLCH()->setTerminationTime(time); ///added by taha
		parent->getLCH()->setTurnaroundDuration(parent->getLCH()->getTerminationTime() - parent->getLCH()->getArrivalTime());
		//TotalTRT += parent->getLCH()->getTurnaroundDuration(); //waiting for DR to remove
		
		Process * temp =parent->getLCH();
		sc->Kill(temp);
		parent->setLCH(nullptr);
	}
	if (parent->getLCH())
	{
		KillOrphan(parent->getRCH(), time);
		sc->Trm(parent->getRCH()); 
		
		parent->getRCH()->setTerminationTime(time);
		parent->getRCH()->setTurnaroundDuration(parent->getRCH()->getTerminationTime() - parent->getRCH()->getArrivalTime());
		//TotalTRT += parent->getRCH()->getTurnaroundDuration(); //waiting for DR to remove
		
		Process* temp = parent->getRCH();
		sc->Kill(temp);
		parent->setRCH(nullptr);
	}
}

void FCFS::sigkill(int timestep, int NF)
{
	//if more than one process are to be killed at the same timestep ?
	// Mimo read his note from Amira 
	// resolve the case mentioned above using a while loop

	SNode<int> Q;
	Q = *(killSig.peek());
	int kill = Q.getFirstItem(); // get the kill time
	int ID = Q.getSecondItem(); // get id to be killed
	Node<Process*>* temp = RDY.getHead(); // get head
	while (timestep == kill)
	{
		nf++;
		//check if process is in this processor FCFS[nf] RDY
		while (temp) //traversing the RDY list until I found the process with that ID
		{
			if (temp->getItem()->getPID() == ID)
			{
				sc->setpKill(sc->getpKill() + 1);
				Process* p = temp->getItem();
				KillOrphan(p, timestep); // killing the children and grandchildren and decrementing rdy length
				RDY.deleteNode(p); //delete it from RDY List
				setRDY_Length(getRDY_Length() - p->gettimeRemaining()); //adjusting RDY_Length
				p->setTerminationTime(timestep);
				p->setTurnaroundDuration(p->getTerminationTime() - p->getArrivalTime());
				//TotalTRT += p->getTurnaroundDuration(); //taha - 7asab ans el DR
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
				sc->setpKill(sc->getpKill() + 1);
				KillOrphan(getCurrRun(), timestep); // killing the children and grandchildren
				getCurrRun()->setTerminationTime(timestep);
				getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
				//TotalTRT += getCurrRun()->getTurnaroundDuration(); //taha - 7asab ans el DR
				sc->Trm(getCurrRun()); //terminate this process
				setCurrRun(nullptr);// RUN CURRENTLY EMPTY
			}
		}

		if (nf == NF)
		{
			killSig.dequeue();
			Q = *(killSig.peek());
			kill = Q.getFirstItem(); // get the kill time
			ID = Q.getSecondItem(); // get id to be killed
			temp = RDY.getHead(); // get head
			nf = 0;
		}
		else
			break;
	}
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
		if (forkP <= forkprob)         //compare the randomly generate dto the one from the input file
		{
			sc->fork(getCurrRun());
		}
		bool migrate = sc->migrationmaxw(getCurrRun(), MaxW, timestep);
		
			if (migrate == true ) //&& RDY.getHead()
			{
				//Process* temp = RDY.getHead()->getItem();
				//RDY.deleteNode();
				//setRDY_Length(getRDY_Length() - temp->gettimeRemaining()); //Rdy length is decremented as a process is removed from rdy
				//setCurrRun(temp);
				//migrate = sc->migrationmaxw(getCurrRun(), MaxW, timestep);
				setCurrRun(NULL);
				continue;
			}
		//handling blk
		// the line below is equivalent to :
		// if IO_request time == cpu time - time remaining = time passed since run started
		// /////////////////////////////////taha///////////////////////////
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
		getCurrRun()->execute(timestep); //execute
		if (getCurrRun()->getisFinished())
		{
			Process* p =  getCurrRun();
			KillOrphan(getCurrRun(), timestep);
			//getCurrRun()->setTerminationTime(timestep);
			getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
			//TotalTRT += getCurrRun()->getTurnaroundDuration(); // waiting for DR to remove 
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
		int t = getOverheatTime(); // processor 1
		setOverheatTime(getOverheatTime() - 1);
		if (getOverheatTime() == 0) setisOverheated(false);
	}
	else
	{
		//sigkill(timestep, sc->getNF());
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
				if (getCurrRun()->getnumIO() != 0) // taha 8ayaro b new condtion
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

Process* FCFS:: eject()
{
	Process* temp = RDY.getHead()->getItem();
	RDY.deleteNode();
	/*setRDY_Length(getRDY_Length() - temp->gettimeRemaining());*/
	return temp;
}

void FCFS:: addToReadyQueue(Process* process) //inserting a process to the RDY 
{
	if (process)
	{
		RDY.insertNode(process);
		setRDY_Length(getRDY_Length() + process->gettimeRemaining());
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

FCFS :: ~FCFS()
{}

SQueue<int> FCFS::killSig;

char FCFS::Ptype = 'f';
double FCFS::forkprob = 0;
int FCFS::MaxW = 0;
int FCFS::nf = 0;
