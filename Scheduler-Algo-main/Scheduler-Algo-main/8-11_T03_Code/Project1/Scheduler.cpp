#include "Scheduler.h"

Scheduler::Scheduler()
{
	OverheatConstant = 0;
	ArrP = new Processor * [NF + NR + NS + ND];
	pr = new RR(this);
	pf = new FCFS(this);
	ps = new SJF(this);
	pUI = new UI;
	pd = new EDF(this);
	minflag = false;
	maxflag = false;
	isFileLoaded = true;
	NF = 0;
	NR = 0;
	NS = 0;
	ND = 0;
	pSteal = 0;
	TimeStep = 1;
	char type = 0;
}
void Scheduler::Simulation()
{
	LoadFile();
	NumProcessor = NF + NS + NR + ND;
	full();
	int mode;
	mode = pUI->ReadMode();
	cin.ignore();  //Clear any leftover characters in the input buffer
	// create processors array
	bool isallterminated = allTerminated();	//simulation is working // btba b 0 fl awl
	if (mode == 3)
		pUI->printBeforeSim(); // only condition for mode 3 before simulation 
	while (!isallterminated && isFileLoaded)
	{
		srand(time(0));
		int random = rand() % 100;
		int r = rand() % NumProcessor;
		if (random < 10)
		{
			Overheat(ArrP[r]);
		}
		NewtoRdy(TimeStep);
		
		for (int i = 0; i < NumProcessor; i++)
		{
			ArrP[i]->ScheduleAlgo(TimeStep);
		}
		BlktoRdy();
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
			Sleep(700);		//Wait for second
		}
		incrementTimeStep(); // finally increment time step for next loop
	}
	if (mode == 3)  //no need for bring inside loop as it is printed once
		pUI->printAfterSim();
	    OutputFile();
}
void Scheduler::Overheat(Processor* p)
{
	p->setisOverheated(true);
	AddtoRdy(p->getCurrRun());
	p->setCurrRun(nullptr);
	p->setOverheatTime(OverheatConstant);
	int k = 0;
	while (p->getRDYCount() > 0) 
	{
		AddtoRdy(p->eject());  //needs dynamic casting?????
		//NewQueue.enqueue(p->eject());
		k++;
	}
}
//////////////////////////////////taha////////////////////////
void Scheduler::Steal()
{
	if (TimeStep % STL == 0)
	{
		Processor* shortest = getMinProcessor(1, 0);
		Processor* longest = getMaxProcessor();
		if (shortest)
		{
			if (longest->getRDY_Length() != 0)
			{
				if (minflag == false && maxflag == false)
				{
					double Plimit = ((longest->getRDY_Length() - shortest->getRDY_Length()) / longest->getRDY_Length()) * 100;
					while (Plimit > 40)
					{
						Process* x = longest->gettop();
						if (shortest && longest && x)
						{
							pSteal++;
							shortest->addToReadyQueue(x);
						}
						shortest = getMinProcessor(1, 0);
						longest = getMaxProcessor();
						int t = longest->getRDY_Length(); //testing 
						int m = shortest->getRDY_Length(); //testing
						if (longest->getRDY_Length() != 0)
						{
							Plimit = ((longest->getRDY_Length() - shortest->getRDY_Length()) / longest->getRDY_Length()) * 100;
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

///////////////////Added by Amira //////////////////
void Scheduler::fork(Process* parent)
{
	if (!parent->getLCH() || !parent->getRCH())  //There is <2 children
	{
		Process* child = new Process(TimeStep, NumProcess + 1, parent->gettimeRemaining());
		//Process* child = new Process(TimeStep, NumProcess +1, parent -> gettimeRemaining(), 0 ,  )
		pFork++; // for output file
		NumProcess++;
		Processor* f = getMinProcessor('f', 0);
		f->addToReadyQueue(child);
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
////////////////////////////////////////////////////

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

int Scheduler::getNF()
{
	return NF;
}
void Scheduler::incrementTimeStep()
{
	TimeStep++;
}
int Scheduler::getTimeStep()
{
	return TimeStep;
}
bool Scheduler::allTerminated()
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
		/*int k = n;
		Processor* min = ArrP[n];
		while (min->getisOverheated() && k <= NumProcessor) 
		{
			k++;
			min = ArrP[k];
		}*/
	//Processor* min = ArrP[n];
	//int w = 0;
	while (n < NumProcessor && ArrP[n]->getisOverheated())
		n++;
	if (n == NumProcessor)
		return NULL ;

		Processor* min = ArrP[n];
	for (int i = n +1; i < NumProcessor; i++)
	{
		if (ArrP[i]->getisOverheated())
			continue;
		if (a == 'f' && i == NF) break;
		if (a == 's' && i == NF + NS) break;
		if (a == 'r' && i == NF + NS + NR) break;
		if (ArrP[i]->getRDY_Length() < min->getRDY_Length())
			min = ArrP[i];


	}



	//Processor* min = NULL;
	//for (int i = n; i < NumProcessor; i++)
	//{
	//	int mini = INFINITE;
	//	if (a == 'f' && i == NF) break;
	//	if (a == 's' && i == NF + NS) break;
	//	if (a == 'r' && i == NF + NS + NR) break;
	//	if (ArrP[i]->getRDY_Length() < mini && ArrP[i]->getisOverheated() == false)
	//	{
	//		min = ArrP[i];
	//		mini = ArrP[i]->getRDY_Length();
	//		//int awel = ArrP[0]->getRDY_Length(); // 1 //over heated
	//		//int tany = ArrP[1]->getRDY_Length(); // 1
	//		//int talet = ArrP[2]->getRDY_Length();// 1
	//		//int rabe3 = ArrP[3]->getRDY_Length();// 1
	//	}

	//}
	for (int i = n; i < NumProcessor; i++)
	{
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
		SQueue <int>* amira = BLKQueue.getHead()->getItem()->getIOqueue();
		int ionum = BLKQueue.getHead()->getItem()->getIOqueue()->peekR()->getSecondItem();
		int rem_io_time = ionum - 1;
		BLKQueue.getHead()->getItem()->getIOqueue()->peekR()->setSecondItem(rem_io_time);
		//int ahmed = BLKQueue.getHead()->getItem()->getIOqueue().peek()->getSecondItem();
		if (rem_io_time == 0)
		{
			//int in = BLKQueue.getHead()->getItem()->getIOqueue()->peekR()->getFirstItem();// testing 
			BLKQueue.getHead()->getItem()->getIOqueue()->dequeueR();
			//int i = BLKQueue.getHead()->getItem()->getIOqueue()->peekR()->getFirstItem(); // testing
			Process* temp;
			BLKQueue.dequeue(temp);
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
	string createFile = "";
	string path = "/8-11_T03_Code\Project1\OutputFiles";
	OutputFile.open("OutputFiles/" + path + ".txt", ios::out);
	//OutputFile.open(createFile.c_str());
	//OutputFile.open("OutputFiles.txt" );
	OutputFile << "TT\tPID\tAT\tCT\tIO_D\tWT\tRT\tTRT\tDL" << endl;
	//int a1 = ArrP[0] ->getRDY_Length(); // testing
	//int  a2 = ArrP[1]->getRDY_Length(); // testing 
	//int  a3 = ArrP[2]->getRDY_Length(); // testing 
	//int  a4 = ArrP[3]->getRDY_Length(); // testing 
	while (TerminatedQueue.dequeueOT(process)) //printing finished processes
	{
		process->getTotalIO_D();//testing 
		process->setwaitingtime(process->getTurnaroundDuration() - process->getCpuTime());
		OutputFile << process->getTerminationTime() << "\t" << process->getPID() << "\t" << process->getArrivalTime() ;
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
	OutputFile << "early deadlines %: " << (earlyDeadLine *100)/ NumProcess << " %" << endl;
	OutputFile << "Processes: " << NumProcess << endl;
	OutputFile << "Avg WT = " << Total_WT / NumProcess << "\t" << "Avg RT = " << Total_RT / NumProcess << "\t" << "Avg TRT = " << Total_TRT / NumProcess <<endl;
	OutputFile << "Migration %:" << "\t" << "RTF=" << (pRTF / NumProcess) * 100 << " %\t" << "MaxW = " << (pMaxW / NumProcess) * 100 << " %" << endl;
	OutputFile << "Work Steal%: " << (pSteal / NumProcess) << "%" << endl;
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

	ifstream inputFile("input.txt");
	string line;   // lama ne3mel input file ha4el comment 
	if (inputFile.fail())
	{
		cout << "Cannot load input file!!" << endl;    // Check if file is not opened
		isFileLoaded = false;
		exit(1); // terminates program

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

}

Scheduler::~Scheduler()
{
	/*for (int i = 0; i < NumProcessor; i++)
	{
		delete ArrP[i];
	}
	delete[] ArrP;*/
	if (pUI)
		delete pUI;
}
void Scheduler::NewtoRdy(int timestep) 
{
	Process* temp;
	bool isalloverheated = false;
	int count = 0;
	for (int i = 0; i < NumProcessor; i++)
	{ 
		if (ArrP[i]->getisOverheated() == true)
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
	for (int i = 0; i < NumProcessor;i++)
	{
		if (ArrP[i]->getPtype() == 'f')
		{
			if (ArrP[i]->getCurrRun() == orphan)
			{
				Trm(orphan);
				ArrP[i]->setCurrRun(nullptr);
			}
			bool found = ((FCFS*)ArrP[i])->isfound(orphan);
			if (found)
			{
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
	int t = TimeStep; // testing 

	Processor* min;
	min = getMinProcessor(1, 0);
	if (min)
	{
		if (SJF* tmp = dynamic_cast<SJF*>(min))
		{
			Process* t = temp; // testing
			min->addToReadyQueue(temp);

		}
		else if (RR* tmp = dynamic_cast<RR*>(min))
		{
			Process* te = temp; // testing
			min->addToReadyQueue(temp);
		}
		else if (FCFS* tmp = dynamic_cast<FCFS*>(min))
		{
			Process* ter = temp; // testing 
			min->addToReadyQueue(temp);
		}
		else
			min->addToReadyQueue(temp);
	}
}

//////////////////////ADDEDDDDDDDDDDDDDD BY MIMOOOOOOOOOOOOOOOOOOO/////////////////////////
bool Scheduler::migrationrtf(Process* p, int rtf) // bool 34an a3rf a remove mn run wla la
{
	if (minflag == false)
	{
		if (!p->getorphanflag())
		{
			if (p->gettimeRemaining() < rtf)
			{
				pRTF++; // for output file
				Processor* min;
				min = getMinProcessor('s', NF);
				if (NS != 0)
				{
					min->addToReadyQueue(p);  // add to shortest sjf
					return true;
				}
			}
		}
	}
	return false;
}

bool Scheduler::migrationmaxw(Process* p, int maxw, int timestep) // bool 34an a3rf a remove mn run wla la
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
				pMaxW++; // for output file
				Processor* min;
				min = getMinProcessor('r', NF + NS);
				if (NR != 0)
				{
					min->addToReadyQueue(p);  // add to shortest sjf
					return true;
				}
			}
		}
	}
	return false;
}
double Scheduler::getpKill()
{
	return pKill;
}
void Scheduler::setpKill(int n)
{
	pKill = n;
}
