#include "Scheduler.h"

	Scheduler::Scheduler()
	{
		ps = new SJF;
		pUI = new UI;
		isFileLoaded = false;
		fiveStepCounter = 0;
		NF= 0;
		NR= 0;
		NS = 0;
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
