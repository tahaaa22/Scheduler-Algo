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

void FCFS::KillOrphan(Process* subRoot, int time)
{
	if ((!subRoot->getLCH() && !subRoot->getRCH()) || !subRoot) //no children or no parent
	{
		return;
	}
	if (subRoot->getLCH())
	{
		KillOrphan(subRoot->getLCH(), time);
		subRoot->getLCH()->setwaitingtime(subRoot->getLCH()->gettimeRemaining());
		subRoot->getLCH()->setTerminationTime(time); ///added by taha
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

void FCFS::sigkill(int timestep, int NF, int pnow)
{
	SNode<int> Q;
	if (killSig.isEmpty()) /// lw empty return 3ltol mnlf4 
		return;
	Q = *(killSig.peek());
	bool found = false; // did i found el 3yza a killo wla la?
	int loop = 0; // how many times ana fdlt fy nfs el processor 
	int killtime = Q.getFirstItem(); // Get the kill time
	int ID = Q.getSecondItem(); // Get id to be killed
	while (timestep == killtime)
	{
		nf++; // ana lafat 3la kam processor 
		Node<Process*>* temp = RDY.getHead(); // Get head
		loop++; // ana blf fy el processor aho f add it
		//check if process is in this processor FCFS[nf] RDY
		while (temp) //traversing the RDY list until I found the process with that ID
		{
			if (temp->getItem()->getPID() == ID)
			{
				sc->setpKill(sc->getpKill() + 1);
				Process* p = temp->getItem();
				KillOrphan(p, timestep); // killing the children and grandchildren and decrementing rdy length
				p->setiskilled(true);
				p->setwaitingtime(p->gettimeRemaining());
				setRDY_Length(getRDY_Length() - p->gettimeRemaining()); //adjusting RDY_Length
				//////////////////////////AMIRA////////////////////////////////////////////
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
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				RDY.deleteNode(p); //delete it from RDY List
				p->setTerminationTime(timestep);
				p->setTurnaroundDuration(p->getTerminationTime() - p->getArrivalTime());
				//TotalTRT += p->getTurnaroundDuration(); //taha - 7asab ans el DR
				sc->Trm(p); //terminate this process
				found = true;
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
				getCurrRun()->setwaitingtime(getCurrRun()->gettimeRemaining());
				getCurrRun()->setiskilled(true);
				getCurrRun()->setTerminationTime(timestep);
				getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
				//TotalTRT += getCurrRun()->getTurnaroundDuration(); //taha - 7asab ans el DR
				sc->Trm(getCurrRun()); //terminate this process
				setCurrRun(nullptr);// RUN CURRENTLY EMPTY
				found = true;
			}
		}

		if (nf == NF) /// lw ana lafat 3la kol el fcfs w mal2to4 5las n dequeue it 
		{
			killSig.dequeue();
			if (killSig.isEmpty())
				break;
			Q = *(killSig.peek());
			killtime = Q.getFirstItem(); // get the kill time
			ID = Q.getSecondItem(); // get id to be killed
			nf = 0; // check from the first processor again
			found = false;

		}
		else if (found) // lw ana lafat w la2to killato dequeue it ba 34an a check el b3do
		{
			killSig.dequeue();
			if (killSig.isEmpty())
				break;
			Q = *(killSig.peek());
			killtime = Q.getFirstItem(); // get the kill time
			ID = Q.getSecondItem(); // get id to be killed
			nf = 0; //lesa added now
			found = false;
		}
		// lw ana blf fy nfs el processor ll tany mra w mal2to4 nady el processor el tanyan
		else if (loop != 1 && !found)
		{
			// hn loop 3la el fcfs processor nnady 3la sigkill bt3thom m3da el ana fy now
			for (int i = 0; i < NF; i++)
			{
				FCFS* temp = sc->GetFCFSprocessor(i); // Getting the fcfs processor el ana 3yzah
				if (i != actualp - 1 && i != pnow - 1) // checking eny msh d5la el ana fy mn el awl or el ana hba fy kman 4owya tany
				{
					pnow = i + 1; // el ana hba fy kman 4owya 
					temp->sigkill(timestep, NF, pnow);
				}
			}
			break; //lafat 3la kolo 5las

		}
		else
		{
			break; // msh blf tany + el element dh msh mawgod 3ndy 
		}

	}
	if (actualp == NF)
	{
		actualp = 0;
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
				//Process* temp = RDY.peek();
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
		if (getCurrRun()->getfirsttime() == 1)
			getCurrRun()->setfirsttime(2);
		else if (getCurrRun()->getfirsttime() == 2)
			getCurrRun()->execute(timestep); //execute
		////////////////////////////////////////////////////////////////////////////

		if (getCurrRun()->getisFinished())
		{
			if (getCurrRun()->getorphanflag() == true)
				//Process* p =  getCurrRun();
			{
				KillOrphan(getCurrRun(), timestep);
			}
			getCurrRun()->setTerminationTime(timestep);
			getCurrRun()->setTurnaroundDuration(getCurrRun()->getTerminationTime() - getCurrRun()->getArrivalTime());
			getCurrRun()->setwaitingtime(getCurrRun()->getTurnaroundDuration() - getCurrRun()->getCpuTime());// to make sure waiting time of killed orphans not with negative value
			//TotalTRT += getCurrRun()->getTurnaroundDuration(); // waiting for DR to remove 
			sc->Trm(getCurrRun());
			setCurrRun(nullptr);
		}

		break;
	}
}


void FCFS::ScheduleAlgo(int timestep)
{
	actualp++;
	currenttime = timestep;
	if (getisOverheated())
	{
		int t = getOverheatTime(); // processor 1
		setOverheatTime(getOverheatTime() - 1);
		if (getOverheatTime() == 0) setisOverheated(false);
	}
	else
	{
		sigkill(timestep, sc->getNF(), sc->getNF() + 1);
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

Process* FCFS:: eject()
{
	Process* temp = RDY.peek();
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

FCFS :: ~FCFS() {}

SQueue<int> FCFS::killSig;

char FCFS::Ptype = 'f';
double FCFS::forkprob = 0;
int FCFS::MaxW = 0;
int FCFS::nf = 0;
int FCFS::actualp = 0;