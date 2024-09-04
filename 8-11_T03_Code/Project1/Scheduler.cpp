#include "Scheduler.h"

Scheduler::Scheduler()
{
	OverheatConstant = 0;
	ArrP = new Processor * [NF + NR + NS + ND]; //Array of Processor pointers
	pr = new RR(this); //pointer of RR
	pf = new FCFS(this); //pointer of FCFS
	ps = new SJF(this); //pointer of SJF
	pUI = new UI;  //Pointer of UI
	pd = new EDF(this); //pointer of EDF
	minflag = false; //used in stealing and migration
	maxflag = false; //used in stealing and migration
	isFileLoaded = true;
	NF = 0; //number of FCFS processors
	NR = 0; //number of RR processors
	NS = 0; //number of SJF processors
	ND = 0; //number of EDF processors
	pSteal = 0; //number of stolen processes
	TimeStep = 0;
	char type = 0;
	Blkcounter = 0;
}

Scheduler::~Scheduler()
{
	// Housekeeping
	delete pr;
	delete pf;
	delete ps;
	delete pd;
	delete pUI;
	delete Pp;
	for (int i = 0; i < NumProcessor; i++) //deleting array of processors
	{
		delete ArrP[i];
	}
	while (!TerminatedQueue.isEmpty())  //freeing the termination queue
	{
		Process* del;
		TerminatedQueue.dequeue(del);
		delete del;
	}
}

void Scheduler::Simulation()
{
	LoadFile();
	NumProcessor = NF + NS + NR + ND;
	if (NumProcessor == 0)
	{
		pUI->printwarning();
		pUI->printending();
		return;
	}
	full(); //filling the Array of processor pointers
	int mode;
	//pUI->printwelcome();
	mode = pUI->ReadMode();
	cin.ignore();  //Clear any leftover characters in the input buffer
	// create processors array
	bool isallterminated = allTerminated();
	if (mode == 3)
		pUI->printBeforeSim(); // only condition for mode 3 before simulation 
	while (!isallterminated && isFileLoaded) //processes are not all terminated and the input file is loaded
	{
		srand(time(0));
		int random = rand() % 100;
		int r = rand() % NumProcessor; //generating random number as overheat probability
		if (random < 10)
		{
			//Overheat(ArrP[r]); //overheating
		}
		NewtoRdy(TimeStep); //distribute processes from New to Ready
		for (int i = 0; i < NumProcessor; i++)
		{
			ArrP[i]->ScheduleAlgo(TimeStep); //running the scheduleAlgo for all processors
		}
		BlktoRdy(); //taking the processes that finished their IO_D to Ready
		KillSignal();
		Steal();
		isallterminated = allTerminated();
		// NOW CHOOSING MODES////
		if (mode == 1)
		{
			// PRINTING //
			Output(TimeStep);
			getchar();	// Waits for user to press "Enter" 
		}
		else if (mode == 2)
		{
			// PRINTING //
			Output(TimeStep);
			Sleep(50);		//Wait for second
		}
		incrementTimeStep(); // finally increment time step for next loop
	}
	if (mode == 3)  //no need for bring inside loop as it is printed once
		pUI->printAfterSim();
    pUI->printending();
	OutputFile();
}

void Scheduler::Overheat(Processor* p)
{
	int counter = 0; // to check if all processors of type FCFS are overheated
	bool allFCFSOverheated = false; // to check of all FCFS processors are overheated or not
	p->OverheatRun(OverheatConstant);
	while (p->getRDYCount() > 0)
	{
		Process* temp = p->eject(); //ejecting the process at top of RDY queue
		if (getMinProcessor(1, 0) == NULL)// if all processors are overheated all processes in the ready and the run will be terminated
			Trm(temp); //terminate the process on top
		else
		{
			while (counter < NF && ArrP[counter]->getisOverheated())
				counter++; //get counter of the first NON overheated FCFS
			if (counter == NF)
				allFCFSOverheated = true;
			if (allFCFSOverheated && getMinProcessor('f', 0) && temp->getorphanflag())
				Trm(temp); //terminate the forked if all FCFS processors are overheated
			else
				AddtoRdy(temp);
		}
	}
}

void Scheduler::Steal()
{
	if (TimeStep % STL == 0) //stealing every [constant] timesteps
	{
		Processor* shortest = getMinProcessor(1, 0);
		Processor* longest = getMaxProcessor();
		if (shortest)
		{
			if (longest->getRDY_Length() != 0)
			{
				if (minflag == false && maxflag == false)
				{
					double Plimit = ((longest->getRDY_Length() - shortest->getRDY_Length()) / (double)longest->getRDY_Length()) * 100;
					if (TimeStep == 30)
						int t = 0;
					while (Plimit > 40) //check if steal limit is reached
					{
						if (TimeStep == 19)
							int t = 0;
						Process* x = longest->gettop();
						if (x == NULL)
							break;
						if (shortest && longest && x)
						{
							pSteal++;
							shortest->addToReadyQueue(x, TimeStep);
						}
						/*shortest = getMinProcessor(1, 0);
						longest = getMaxProcessor();*/
						if (shortest->getRDY_Length() > longest->getRDY_Length())
							break;
						if (longest->getRDY_Length() != 0) // to make sure that Plimit is not 100 precent when shortest is zero length
						{
							Plimit = ((longest->getRDY_Length() - shortest->getRDY_Length()) / (double)longest->getRDY_Length()) * 100;
						}
						if (minflag == true || maxflag == true)
							break;
						if (shortest->getRDY_Length() == 0)
							break;

					}
				}
			}
		}
	}
}


void Scheduler::fork(Process* parent)
{
	if (!parent->getLCH() || !parent->getRCH())  //There are < 2 children
	{
		Process* child = new Process(TimeStep, NumProcess + 1, parent->gettimeRemaining(), parent);
		pFork++; // for output file
		NumProcess++;
		Processor* f = getMinProcessor('f', 0);
		f->addToReadyQueue(child, TimeStep);
		//maintaining a fork tree
		if (!parent->getLCH())
		{
			parent->setLCH(child);
			return;
		}
		if (!parent->getRCH())
		{
			parent->setRCH(child);
			return;
		}
	}
}

void Scheduler::full()
{

	for (int i = 0; i < NumProcessor; i++)
	{
		if (i < NF)
		{
			FCFS* fcfs = new FCFS(this);
			ArrP[i] = fcfs;
		}
		else if (i < NF + NS)
		{
			SJF* sjf = new SJF(this);
			ArrP[i] = sjf;
		}
		else if (i < NF + NS + NR)
		{
			RR* rr = new RR(this);
			ArrP[i] = rr;
		}
		else
		{
			EDF* df = new EDF(this);
			ArrP[i] = df;
		}
	}
}

void Scheduler::KillSignal()
{
	if (NF > 0)
	{
		bool found = true; //if i didnot find the processor needed to kill - true at the first to enter the while loop
		while (found)
			for (int i = 0; i < NF; i++)
				found = ((FCFS*)ArrP[i])->sigkill(TimeStep, NF);
	}

}

void Scheduler::incrementTimeStep()
{
	TimeStep++;
}

bool Scheduler::allTerminated() //checks if all processes are terminated
{
	return TerminatedQueue.getCount() == NumProcess;
}
Processor* Scheduler::getMaxProcessor()
{
	int count = 0;
	Processor* max = ArrP[0];
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getRDY_Length() > max->getRDY_Length())
		{
			max = ArrP[i];
		}

	}
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getRDY_Length() == max->getRDY_Length())
		{
			count++;
		}
	}
	if (count != 1)
		maxflag = true;
	else
		maxflag = false;
	return max;
}
Processor* Scheduler::getMinProcessor(char a, int n)
{
	int count = 0;
	while (n < NumProcessor && ArrP[n]->getisOverheated())
		n++;
	if (n == NumProcessor)
		return NULL;

	Processor* min = ArrP[n];
	int m1 = min->getRDY_Length();
	for (int i = n + 1; i < NumProcessor; i++)
	{
		if (ArrP[i]->getisOverheated())
			continue;
		if (a == 'f' && i == NF) break;
		if (a == 's' && i == NF + NS) break;
		if (a == 'r' && i == NF + NS + NR) break;
		int m2 = ArrP[i]->getRDY_Length();
		if (ArrP[i]->getRDY_Length() < min->getRDY_Length())
			min = ArrP[i];
	}

	for (int i = n; i < NumProcessor; i++)
	{
		if (ArrP[i]->getisOverheated())
			continue;
		if (a == 'f' && i == NF) break;
		if (a == 's' && i == NF + NS) break;
		if (a == 'r' && i == NF + NS + NR) break;
		if (ArrP[i]->getRDY_Length() == min->getRDY_Length())
			count++;
	}
	if (count != 1)
		minflag = true;
	else
		minflag = false;
	return min;
}


void  Scheduler::BlktoRdy()
{
	if (!BLKQueue.isEmpty())
	{
		int ionum = BLKQueue.getHead()->getItem()->getIOqueue()->peekR()->getSecondItem();
		Blkcounter++; // counter to check if the duration of the IO resources is done or not
		if (Blkcounter == (ionum+1))
		{
			
			BLKQueue.getHead()->getItem()->setTotalIO_D(BLKQueue.getHead()->getItem()->getTotalIO_D() + ionum); // sum of durations that is done for each process for output file 
			Blkcounter = 0;
			BLKQueue.getHead()->getItem()->getIOqueue()->dequeueR();
			Process* temp;
			BLKQueue.dequeue(temp);
			if (getMinProcessor(1, 0) == NULL)
			{
				Trm(temp);
				temp->setwaitingtime(temp->gettimeRemaining());
			}
			else
				AddtoRdy(temp);
		}
	}
}
void Scheduler::OutputFile()
{
	ofstream OutputFile;
	Total_WT = 0;
	Total_RT = 0;
	Total_TRT = 0;
	Avg_Util = 0;
	int earlyDeadLine = 0;
	Process* process;
	string folderName = "OutputFiles";
	string fileName = "output_" + to_string(time(nullptr)) + ".txt";
	string filePath = folderName + "/" + fileName;

	// Open the file for writing
	OutputFile.open(filePath, ios::out);
	OutputFile << "TT\tPID\tAT\tCT\tIO_D\tWT\tRT\tTRT\tDL" << endl;
	while (TerminatedQueue.dequeueOT(process)) //printing finished processes
	{
		if (process->getorphanflag() == false && process->getiskilled() == false)
			process->setwaitingtime(process->getTurnaroundDuration() - process->getCpuTime());
		if (process->getWaitingTime() < 0)
			process->setwaitingtime(process->getWaitingTime() * -1); // if processes terminated before finishing its CPU time due to any action
		OutputFile << process->getTerminationTime() << "\t" << process->getPID() << "\t" << process->getArrivalTime();
		OutputFile << "\t" << process->getCpuTime() << "\t" << process->getTotalIO_D() << "\t" << process->getWaitingTime() << "\t" << process->getResponseTime() << "\t" << process->getTurnaroundDuration();
		OutputFile << "\t" << process->getDeadLine();
		if (process->getTerminationTime() < process->getDeadLine())
			earlyDeadLine++;
		OutputFile << endl;// end of process
		Total_WT += process->getWaitingTime();
		Total_RT += process->getResponseTime();
		Total_TRT += process->getTurnaroundDuration();
	}
	OutputFile << "------------------------------------------------------------------------------------------" << endl;
	OutputFile << endl;
	OutputFile << endl;
	OutputFile << "early deadlines %: " << (earlyDeadLine * 100) / NumProcess << " %" << endl;
	OutputFile << "Processes: " << NumProcess << endl;
	OutputFile << "Avg WT = " << Total_WT / NumProcess << "\t" << "Avg RT = " << Total_RT / NumProcess << "\t" << "Avg TRT = " << Total_TRT / NumProcess << endl;
	OutputFile << "Migration %:" << "\t" << "RTF=" << (pRTF / NumProcess) * 100 << " %\t" << "MaxW = " << (pMaxW / NumProcess) * 100 << " %" << endl;
	OutputFile << "Work Steal%: " << (pSteal / NumProcess) * 100 << "%" << endl;
	OutputFile << "Forked Process: " << (pFork / NumProcess) * 100 << "%" << endl;
	OutputFile << "Killed Process: " << (pKill / NumProcess) * 100 << "%" << endl;
	OutputFile << endl;
	OutputFile << endl;
	OutputFile << "Processors: " << NumProcessor << " [" << NF << " FCFS, " << NS << " SJF, " << NR << " RR, " << ND << " EDF]" << endl;
	OutputFile << "Processor Load" << endl;
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i] != NULL)
		{
			if (i < NF)
			{
				double busytime = ArrP[i]->getTotalBusy();
				OutputFile << "P" << i + 1 << "=" << (busytime / Total_TRT) * 100 << "%,\t";
			}
			else if (i < NF + NS)
			{
				double busytime = ArrP[i]->getTotalBusy();
				OutputFile << "P" << i + 1 << "=" << (busytime / Total_TRT) * 100 << "%,\t";
			}
			else if (i < NF + NS + NR)
			{
				double busytime = ArrP[i]->getTotalBusy();
				OutputFile << "P" << i + 1 << "=" << (busytime / Total_TRT) * 100 << "%,\t";
			}
			else
			{
				double busytime = ArrP[i]->getTotalBusy();
				OutputFile << "P" << i + 1 << "=" << (busytime / Total_TRT) * 100 << "%\t";
			}
		}
	}
	OutputFile << endl;
	OutputFile << "Processor Utiliz" << endl;
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i] != NULL)
		{
			if (i < NF)
			{
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pUtil() * 100 << "%,\t";
				Avg_Util += ArrP[i]->pUtil();
			}
			else if (i < NF + NS)
			{
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pUtil() * 100 << "%,\t";
				Avg_Util += ArrP[i]->pUtil();
			}
			else if (i < NF + NS + NR)
			{
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pUtil() * 100 << "%,\t";
				Avg_Util += ArrP[i]->pUtil();
			}
			else
			{
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pUtil() * 100 << "%\t";
				Avg_Util += ArrP[i]->pUtil();
			}
		}
	}
	OutputFile << endl;
	OutputFile << "Avg utilization = " << (Avg_Util / NumProcessor) * 100 << "%" << endl;
	OutputFile.close();
}
void Scheduler::Output(int time)
{
	pUI->Print1(TimeStep, NumProcessor, ArrP, NF, NS, NR, ND);
	pUI->Print2(BLKQueue, BLKQueue.getCount());
	pUI->Print3(NumProcessor, ArrP);
	pUI->Print4(TerminatedQueue, TerminatedQueue.getCount());
}
void Scheduler::LoadFile()
{

	string inputFilePath = "input files\\steal.txt"; // full path to input file
	ifstream inputFile(inputFilePath);
	bool isFileLoaded;

	if (inputFile.fail())
	{
		cout << "Cannot load input file!!" << endl;
		isFileLoaded = false;
		exit(1);
	}
	else
	{
		isFileLoaded = true;
	}
	//------------------Line 1-----------------------------//


	inputFile >> NF;	// Number of FCFS processor
	inputFile >> NS;		// Number of SJF processor
	inputFile >> NR;		// Number of RR processor
	inputFile >> ND; // Number of ND processor
	//------------------Line 2-----------------------------//
	// for loading overheat duration 
	inputFile >> OverheatConstant;
	//------------------Line 3-----------------------------//
	// Skip the next line
	pr->Loadp(inputFile);
	//------------------Line 4-----------------------------//
	pf->Loadp(inputFile);
	inputFile >> STL;

	pf->Loadpf(inputFile);

	//------------------Line 5-----------------------------//
	inputFile >> NumProcess;		// num of processes
	//------------------Line 6-----------------------------//
	//create Processes//

	for (int i = 0; i < NumProcess; i++)
	{
		Pp = new Process;
		Process* p = Pp->load(inputFile);
		NewQueue.enqueue(p);
	}
	//------------------Line 6-----------------------------//
	pf->Loadkill(inputFile);
	inputFile.close();
}


void Scheduler::NewtoRdy(int timestep) //Adds processes from new queue to RDY queue
{
	Process* temp;
	bool isalloverheated = false;
	int count = 0;
	for (int i = 0; i < NumProcessor; i++)
	{
		bool heat = ArrP[i]->getisOverheated();
		if ( heat== true)
			count++;
	}
	if (count == NumProcessor)
		isalloverheated = true;
	
	while (NewQueue.getHead() && (NewQueue.getHead()->getItem()->getArrivalTime() == timestep) && isalloverheated == false)
	{
		NewQueue.dequeue(temp);
		AddtoRdy(temp);
	}
}

void Scheduler::Kill(Process* orphan)
{
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getPtype() == 'f') //FCFS processor
		{
			((FCFS*)ArrP[i])->KillRun(orphan); //if orphan is in RUN
			bool found = ((FCFS*)ArrP[i])->isfound(orphan); //if orphan is in RDY
			if (found)
			{
				cout << endl;
				cout << "orphan " << orphan << " from ready terminated\n" << endl;
				Trm(orphan);
				((FCFS*)ArrP[i])->deleteNodeK(orphan);
			}
		}
	}
}

void Scheduler::Trm(Process* p)
{
	p->setisFinished(true);
	TerminatedQueue.enqueue(p);
}

void Scheduler::RuntoBlk(Process* p) {
	p->setisBlocked(true);
	BLKQueue.enqueue(p);
}

void Scheduler::AddtoRdy(Process* temp)
{
	Processor* min;
	min = getMinProcessor(1, 0);
	if (min)
	{
		min->addToReadyQueue(temp, TimeStep);
	}
}


bool Scheduler::migrationrtf(Process* p, int rtf) // bool to know whether to remove from RUN or not
{
	if (minflag == false)
	{
		if (!p->getorphanflag()) //not a forked process
		{
			if (p->gettimeRemaining() < rtf)
			{
				
				Processor* min;
				min = getMinProcessor('s', NF);
				if (NS != 0 && min)
				{
					pRTF++; // for output file
					min->addToReadyQueue(p, TimeStep);  // add to shortest sjf
					return true;
				}
			}
		}
	}
	return false;
}

bool Scheduler::migrationmaxw(Process* p, int maxw, int timestep) // bool to know whether to remove from RUN or not
{
	int timerunned = p->getCpuTime() - p->gettimeRemaining();
	int at = p->getArrivalTime();
	int waitingtime = timestep - at - timerunned;
	if (minflag == false)
	{
		if (!p->getorphanflag())
		{
			if (waitingtime > maxw)
			{
				
				Processor* min;
				min = getMinProcessor('r', NF + NS);
				if (NR != 0 && min)
				{
					pMaxW++; // for output file
					min->addToReadyQueue(p, TimeStep);  // add to shortest RR
					return true;
				}
			}
		}
	}
	return false;
}
double Scheduler::getpKill() //get number of actual killed processes
{
	return pKill;
}
void Scheduler::setpKill(int n) //set number of actual killed processes
{
	pKill = n;
}