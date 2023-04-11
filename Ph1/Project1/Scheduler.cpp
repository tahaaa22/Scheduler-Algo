#include "Scheduler.h"

	Scheduler::Scheduler()
	{
		// mimo is here
		ps = new SJF;
		pUI = new UI;
		isFileLoaded = false;
		fiveStepCounter = 0;
		NF= 0;
		NR= 0;
		NS = 0;
		TimeStep = 0;
	}

	void Scheduler:: incrementTimeStep() {
		TimeStep++;
	}

	int Scheduler::getTimeStep() {
		return TimeStep;
	}

	void Scheduler::TerminateAll() {

		if(TerminatedQueue.getCount()== NumProcess)
			// what?
	}

	Processor* Scheduler::getMaxProcessor() {
		Processor* max = ArrP[0];
		for (int i = 0; i < NS+NF+NR; i++)
		{
			if (ArrP[i]->getRDY_Length() > max->getRDY_Length())
				max = ArrP[i];
		}
		return max;
	}

	Processor* Scheduler::getMinProcessor() {
		Processor* min = ArrP[0];
		for (int i = 0; i < NS + NF + NR; i++)
		{
			if (ArrP[i]->getRDY_Length() < min->getRDY_Length())
				min = ArrP[i];
		}
		return min;
	}

	double Scheduler::StealLimit() {
		int shortest = getMinProcessor()->getRDY_Length();
		int longest = getMaxProcessor()->getRDY_Length();
		return (longest - shortest) / longest;
	}

	bool Scheduler::HandleBlk() {

	}

	

	void Scheduler::Mode()
	{}
	/*void Scheduler::Output(int currenttime) // function used to relate to the printing functions for each state from UI class.
	{}*/

	void Scheduler::LoadFile()
	{
		ifstream infile;
		//infile.open("ay file ba2a");// lama ne3mel input file ha4el comment 
		if (infile.fail())
		{
			cout << "Cannot load input file!!" << endl;    // Check if file is not opened
			exit(1); // terminates program
		}
		else
		{
			isFileLoaded = true;
		}
		//------------------Line 1-----------------------------//
		infile >> NF;	// Number of FCFS processor
		infile >> NS;		// Number of SJF processor
		infile >> NR;		// Number of RR processor

		//------------------Line 2-----------------------------//
		infile >> RRtime;		// time slice of RR

		//------------------Line 3-----------------------------//
		infile >> RTF;		// RTF
		infile >> MaxW;		// MaxW
		infile >> STL;		// STL
		infile >> Pfork;    //fork probability

		//------------------Line 4-----------------------------//
		infile >> NumProcess;		// num of processes
		
		//------------------load processes-----------------------------//
		// waiting for new queue//

		//------------------signkill times and ID-----------------------------//
	}
	void Scheduler::addtotermination()
	{
		Process* pT;
		ps->addtoterminate(pT);
		TerminatedQueue.enqueue(pT);
	}
	void Scheduler::addtoBlock()
	{
		Process* pT;
		ps->addtoBLK(pT);
		BLKQueue.enqueue(pT);
	}
	Scheduler::~Scheduler()
	{
		if (pUI)
			delete pUI;
	}
