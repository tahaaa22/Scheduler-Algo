#include "FCFS.h"
#include "Scheduler.h"
using namespace std;

FCFS::FCFS()
{
	settype('f');
}
///////might be removed if not needed/////////////
//bool FCFS::CheckMaxW(Process* p); // used for migration
// int CalcRdyLength();
int FCFS::getRDYCount()
{
	return RDY.getCount();
}
///////////////////////////////////////////

//// setRDY_Length(getRDY_Length() - process->getCpuTime()); Add in migration
	// scheduler fork 

//void FCFS::KillOrphans(Process* parent)
//{
//	if ((!parent->getLCH() && !parent->getRCH()) || !parent)
//	{
//		return;
//	}
//	if (parent->getLCH())
//	{
//		KillOrphans(parent->getLCH());
//		delete parent->getLCH();
//		parent->setLCH(nullptr);
//	}
//	if (parent->getLCH())
//	{
//		KillOrphans(parent->getRCH());
//		delete parent->getRCH();
//		parent->setRCH(nullptr);
//	}
//}

void FCFS::sigkill(int timestep, int NF)
{
	SNode<int> Q;
	Q = killSig.peek();
	int kill = Q.getFirstItem(); // get the kill time
	int ID = Q.getSecondItem(); // get id to be killed
	Node<Process*>* temp = RDY.getHead(); // get head
	if (timestep == kill)
	{
		nf++;
		//check lw el process dy 3ndy hena 
		while (temp) //traversing the RDY list until I found the process with that ID
		{
			if (temp->getItem()->getPID() == ID)
			{
				Process* p = temp->getItem();
				// killing the children and grandchildren
				sc->KillOrphan(p);
				addterminate(p); //terminate this process
				RDY.deleteNode(p); //delete it from RDY List
				setRDY_Length(getRDY_Length() - p->getCpuTime());
				break;
			}
			temp = temp->getNext();
		}
		if (getCurrRun())
		{
			int IDR = getCurrRun()->getPID();
			if (IDR == ID)
			{
				sc->KillOrphan(getCurrRun()); // killing the children and grandchildren
				setRDY_Length(getRDY_Length() - getCurrRun()->getCpuTime());
				addterminate(getCurrRun()); //terminate this process
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

void FCFS::ScheduleAlgo(int time)
{
	if (getCurrRun()) //if current run contains a process
	{
		if (getCurrRun()->getisFinished()) //if the process has finished 
		{
			sc->RuntoTrm(getCurrRun()); // killing the children and grandchildren then terminating
			setCurrRun(nullptr);
		}
		else     //if the process has not finished yet
		{
			getCurrRun()->execute(time);

			//handling fork
			srand(time);
			double forkP = rand() % 100; //generate a random forking probability
			if (forkP <= forkprob)         //compare the randomly generate dto the one from the input file
			{
				sc->fork(getCurrRun());
			}

			//handling blk
			// the line below is equivalent to :
			// if IO_request time == cpu time - time remaining = time passed since run started
			if (getCurrRun()->getIOqueue().peek().getFirstItem() == (getCurrRun()->getCpuTime() - getCurrRun()->gettimeRemaining()))
			{
				sc->RuntoBlk(getCurrRun());
				setCurrRun(nullptr);
			}
		}
	}
	if (!RDY.isEmpty() && !getCurrRun()) //run empty and ready contains processes
	{
		Process* temp = RDY.getHead()->getItem(); //First Process In is at the head, and the turn is on this Process to RUN
		setRDY_Length(getRDY_Length() - temp->getCpuTime()); //Rdy length is decremented as a process is removed from rdy
		RDY.deleteNode(); //deleting first Process as it is removed to RUN 
		setCurrRun(temp); //setting the RUN state to this 
		temp->execute(time);
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
