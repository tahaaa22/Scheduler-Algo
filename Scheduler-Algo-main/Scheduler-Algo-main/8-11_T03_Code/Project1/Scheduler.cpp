#include "Scheduler.h"

Scheduler::Scheduler()
{
	ArrP = new Processor * [NF + NR + NS + ND];
	pr = new RR(this);
	pf = new FCFS(this);
	ps = new SJF(this);
	pUI = new UI;
	pd = new EDF(this);
	isFileLoaded = true;
	NF = 0;
	NR = 0;
	NS = 0;
	ND = 0;
	pSteal = 0;
	TimeStep = 0;
	char type = 0;
	OverheatConstant = 0;
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
		if (random < 5) {
			Overheat(ArrP[r]);
		}
		NewtoRdy(TimeStep);
		for (int i = 0; i < NumProcessor; i++)
			ArrP[i]->ScheduleAlgo(TimeStep);
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
			Sleep(25);		//Wait for second

		}
		incrementTimeStep(); // finally increment time step for next loop
	}
	if (mode == 3)  //no need for bring inside loop as it is printed once
		pUI->printAfterSim();
	    OutputFile();
}
//////////////////////////////////taha////////////////////////
void Scheduler::Steal()
{
	if (STL == TimeStep)
	{
		Processor* shortest = getMinProcessor(1, 0);
		Processor* longest = getMaxProcessor();
		double Plimit = ((longest->getRDY_Length() - shortest->getRDY_Length()) / longest->getRDY_Length()) * 100;
		if (shortest->getRDY_Length())
		{
			while (Plimit > 40)
			{
				Process* x = longest->gettop();
				if (shortest && longest && x && !x->getorphanflag())
				{
					pSteal++;
					shortest->addToReadyQueue(longest->gettop());
				}
				shortest = getMinProcessor(1, 0);
				longest = getMaxProcessor();
				Plimit = ((longest->getRDY_Length() - shortest->getRDY_Length()) / longest->getRDY_Length()) * 100;
				if (shortest->getRDY_Length())
					break;
			}
		}
	}
}

///////////////////Added by Amira //////////////////
void Scheduler::fork(Process* parent)
{
	pFork++; // for output file
	Process* child = new Process(TimeStep, ID_Count + 1, parent->gettimeRemaining());
	if (parent->getLCH() || parent->getRCH())  //There is <2 children
	{
		Processor* f = getMinProcessor('f', 0);
		f->addToReadyQueue(child);
		if (!parent->getLCH())
		{
			parent->setLCH(child);
		}
		if (!parent->getRCH())
		{
			parent->setRCH(child);
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
	Processor* max = ArrP[0];
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getRDY_Length() > max->getRDY_Length())
			max = ArrP[i];
	}
	return max;
}
Processor* Scheduler::getMinProcessor(char a, int n)
{
	int k = n;
	Processor* min = ArrP[n];
	while (min->getisOverheated()) {
		k++;
		min = ArrP[k];
	}
	for (int i = n; i < NumProcessor; i++)
	{
		if (!ArrP[n]->getisOverheated()) {
			if (a == 'f' && i == NF) break;
			if (a == 's' && i == NF + NS) break;
			if (a == 'r' && i == NF + NS + NR) break;
			if (ArrP[i]->getRDY_Length() < min->getRDY_Length())
				min = ArrP[i];
		}
	}
	return min;
}
/*double Scheduler::StealLimit() // m4 me87tagha f 7aga m4 hatenfa3
{
	int shortest = getMinProcessor(1,0)->getRDY_Length();
	int longest = getMaxProcessor()->getRDY_Length();
	return ((longest - shortest) / longest) * 100;
}*/


void  Scheduler::BlktoRdy()
{

	if (!BLKQueue.isEmpty())
	{
		int rem_io_time = BLKQueue.getHead()->getItem()->getIOqueue().peek().getSecondItem() - 1;
		BLKQueue.getHead()->getItem()->getIOqueue().peek().setSecondItem(rem_io_time);
		if (rem_io_time == 0) {
			BLKQueue.getHead()->getItem()->getIOqueue().dequeue();
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
	Process* process;
	OutputFile.open("OutputFiles/" + ofname, ios::out);
	OutputFile << "TT\tPID\tAT\tCT\tIO_D\tWT\tRT\tTRT\tDL" << endl;
	while (TerminatedQueue.dequeueOT(process)) //printing finished processes
	{

		process->setwaitingtime(process->getTurnaroundDuration() - process->getCpuTime());
		OutputFile << process->getTerminationTime() << "\t" << process->getPID() << "\t" << process->getArrivalTime() << "\t";
		OutputFile << "\t" << process->getCpuTime() << "\t" << process->getTotalIO_D() << "\t" << process->getWaitingTime() << "\t" << process->getResponseTime() << "\t" << process->getTurnaroundDuration();
		OutputFile << "\t" << process->getDeadLine();

		OutputFile << endl;// end of process
		Total_WT += process->getWaitingTime();
		Total_RT += process->getResponseTime();
		Total_TRT += process->getTurnaroundDuration();
	}
	OutputFile << "------------------------------------------------------------------------------------------" << endl;
	OutputFile << endl;
	OutputFile << endl;
	OutputFile << "Processes: " << NumProcess << endl;
	OutputFile << "Avg WT = " << Total_WT / NumProcess << "\t" << "Avg RT = " << Total_RT / NumProcess << "\t" << "Avg TRT = " << Total_TRT / NumProcess <<endl;
	OutputFile << "Migration %:" << "\t" << "RTF=" << (pRTF / NumProcess) * 100 << "%\t" << "MaxW = " << (pMaxW / NumProcess) * 100 << endl;
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
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pLoad() * 100 << "%,\t";
			}
			else if (i < NF + NS)
			{
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pLoad() * 100 << "%,\t";
			}
			else if (i < NF + NS + NR)
			{
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pLoad() * 100 << "%,\t";
			}
			else
				OutputFile << "P" << i + 1 << "=" << ArrP[i]->pLoad() * 100 << "%\t";
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
	// Skip the next line
	pr->Loadp(inputFile);
	//------------------Line 3-----------------------------//
	pf->Loadp(inputFile);
	inputFile >> STL;

	pf->Loadpf(inputFile);

	//------------------Line 4-----------------------------//
	inputFile >> NumProcess;		// num of processes
	//------------------Line 5-----------------------------//
	//create Processes//

	for (int i = 0; i < NumProcess; i++)
	{
		Pp = new Process;
		Process* p = Pp->load(inputFile);
		NewQueue.enqueue(p);
	}
	//------------------Line 6-----------------------------//
	inputFile >> OverheatConstant;
	//------------------Line 7-----------------------------//
	pf->Loadkill(inputFile);

}
//void Scheduler::addtoBlock()
//{
//	Process* Pbs = NULL;
//
//	for (int i = 0; i < NumProcessor; i++)
//	{
//		Pbs = ArrP[i]->getBLK();
//		if (Pbs != NULL)
//		{
//			BLKQueue.enqueue(Pbs);
//			ArrP[i]->addtoBLK(NULL);
//		}
//	}
//}
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
void Scheduler::NewtoRdy(int timestep) {
	Process* temp;
	while (NewQueue.getHead() && (NewQueue.getHead()->getItem()->getArrivalTime() == timestep))
	{
		NewQueue.dequeue(temp);
		AddtoRdy(temp);
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

void Scheduler::AddtoRdy(Process* temp) {  //replace command with tmp->addToReadyQueue(temp); ????

	Processor* min;
	min = getMinProcessor(1, 0);
	if (SJF* tmp = dynamic_cast<SJF*>(min)) {
		min->addToReadyQueue(temp);
	}
	else if (RR* tmp = dynamic_cast<RR*>(min)) {
		min->addToReadyQueue(temp);
	}
	else if (FCFS* tmp = dynamic_cast<FCFS*>(min)) {
		min->addToReadyQueue(temp);
	}
	else
		min->addToReadyQueue(temp);
}

//////////////////////ADDEDDDDDDDDDDDDDD BY MIMOOOOOOOOOOOOOOOOOOO/////////////////////////
bool Scheduler::migrationrtf(Process* p, int rtf) // bool 34an a3rf a remove mn run wla la
{
	if (p->gettimeRemaining() < rtf)
	{
		pRTF++; // for output file
		Processor* min;
		min = getMinProcessor('s', NF);
		min->addToReadyQueue(p);  // add to shortest sjf
		return true;
	}
	return false;

}

bool Scheduler::migrationmaxw(Process* p, int maxw, int timestep) // bool 34an a3rf a remove mn run wla la
{
	int timerunned = p->getCpuTime() - p->gettimeRemaining();
	int at = p->getArrivalTime();
	int waitingtime = timestep - at - timerunned;
	if (!p->getorphanflag())
	{
		if (waitingtime > maxw)
		{
			pMaxW++; // for output file
			Processor* min;
			min = getMinProcessor('r', NF + NS);
			min->addToReadyQueue(p);  // add to shortest sjf
			return true;
		}
		return false;
	}

}
double Scheduler::getpKill()
{
	return pKill;
}
void Scheduler::setpKill(int n)
{
	pKill = n;
}

void Scheduler::Overheat(Processor* p) 
{
	p->setisOverheated(true);
	AddtoRdy(p->getCurrRun());
	p->setCurrRun(nullptr);
	p->setOverheatTime(OverheatConstant);
	int k = 0;
	while (p->getRDYCount() > 0) {
		AddtoRdy(p->eject());  //needs dynamic casting?????
		k++;
	}
}
