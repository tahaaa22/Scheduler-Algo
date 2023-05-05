#include "FCFS.h"
#include "Scheduler.h"
using namespace std;

FCFS::FCFS()
{
	settype('f');
}

int FCFS::getRDYCount()
{
	return RDY.getCount();
}

void FCFS::KillOrphan(Process* parent)
{
	if ((!parent->getLCH() && !parent->getRCH()) || !parent) //no children or no parent
	{
		return;
	}
	if (parent->getLCH())
	{
		KillOrphan(parent->getLCH()); //the recursive calls will stop when the subroot has LCH = NULL
		sc->Trm(parent->getLCH()); 
		setRDY_Length(getRDY_Length() - parent->getLCH()->gettimeRemaining());
		delete parent->getLCH();
		parent->setLCH(nullptr); 
	}
	if (parent->getLCH())
	{
		KillOrphan(parent->getRCH());
		setRDY_Length(getRDY_Length() - parent->getLCH()->gettimeRemaining());
		sc->Trm(parent->getRCH());
		delete parent->getRCH();
		parent->setRCH(nullptr);
	}
}

void FCFS::sigkill(int timestep, int NF)
{
	//if more than one process are to be killed at the same timestep ?
	// Mimo read his note from Amira 
	// resolve the case mentioned above using a while loop

	SNode<int> Q;
	Q = killSig.peek();
	int kill = Q.getFirstItem(); // get the kill time
	int ID = Q.getSecondItem(); // get id to be killed
	Node<Process*>* temp = RDY.getHead(); // get head
	if (timestep == kill)
	{
		nf++;
		//check if process is in this processor FCFS[nf] RDY
		while (temp) //traversing the RDY list until I found the process with that ID
		{
			if (temp->getItem()->getPID() == ID)
			{
				Process* p = temp->getItem();
				KillOrphan(p); // killing the children and grandchildren
				RDY.deleteNode(p); //delete it from RDY List
				setRDY_Length(getRDY_Length() - p->getCpuTime()); //adjusting RDY_Length
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
				KillOrphan(getCurrRun()); // killing the children and grandchildren
				sc->Trm(getCurrRun()); //terminate this process
				setCurrRun(nullptr);// RUN CURRENTLY EMPTY
			}

		}
		if (nf == NF)
		{
			killSig.dequeue();
			nf = 0;
		}
	}
}

void FCFS::Handle(int timestep) //this functions executes and checks if the process needs trmination
{
	while (getCurrRun())
	{
		//handling fork
		srand(timestep);
		double forkP = rand() % 100; //generate a random forking probability
		if (forkP <= forkprob)         //compare the randomly generate dto the one from the input file
		{
			sc->fork(getCurrRun());
		}
		bool migrate = sc->migrationmaxw(getCurrRun(), MaxW, timestep);
		if (migrate == true)
		{
			setCurrRun(nullptr);
			continue;
		}
		//handling blk
		// the line below is equivalent to :
		// if IO_request time == cpu time - time remaining = time passed since run started
		int IO_req = getCurrRun()->getIOqueue().peek().getFirstItem();
		int passed = getCurrRun()->getCpuTime() - getCurrRun()->gettimeRemaining();
		if (IO_req == passed)
		{
			sc->RuntoBlk(getCurrRun());
			setCurrRun(nullptr);
			continue;
		}
		getCurrRun()->execute(timestep); //execute
		if (getCurrRun()->getisFinished())
		{
			KillOrphan(getCurrRun());
			sc->Trm(getCurrRun());
			setCurrRun(nullptr);
		}
		break;
	}
}

void FCFS::ScheduleAlgo(int timestep)
{
	sigkill(timestep, sc->getNF());
	Handle(timestep); //equivalent to while run = true (run contains a process)
	while (!getCurrRun()) //while RUN is empty 
	{
		if (!RDY.isEmpty()) //run empty and ready contains processes
		{
			Process* temp = RDY.getHead()->getItem(); //First Process In is at the head, and the turn is on this Process to RUN
			setRDY_Length(getRDY_Length() - temp->getCpuTime()); //Rdy length is decremented as a process is removed from rdy
			RDY.deleteNode(); //deleting first Process as it is removed to RUN 
			setCurrRun(temp);
			Handle(timestep); //handles the current run
		}
	}
}

void FCFS::addToReadyQueue(Process* process) //inserting a process to the RDY 
{
	RDY.insertNode(process);
	setRDY_Length(getRDY_Length() + process->getCpuTime());
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
