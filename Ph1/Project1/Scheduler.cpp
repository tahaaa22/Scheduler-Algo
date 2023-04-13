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

	bool Scheduler:: allTerminated()
	{
		 return TerminatedQueue.getCount() == NumProcess;
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
	{
		LoadFile();
		int mode;
		mode = pUI->ReadMode();
		cin.ignore();  //Clear any leftover characters in the input buffer
		if (mode == 1)
		{
			bool isallterminated = allTerminated();	//simulation is working

			while (!isallterminated && isFileLoaded)
			{
				addtotermination();
					Simulation(TimeStep);
					isallterminated = allTerminated(); 
					fiveStepCounter++;
				// PRINTING //
				output(TimeStep);
				getchar();	// Waits for user to press "Enter" 
				incrementTimeStep();
			}
		}//exit of while loop(program)
		else if (mode == 2)
		{
			bool isallterminated = allTerminated();	//simulation is working

			while (!isallterminated && isFileLoaded)
			{
				addtotermination();
				Simulation(TimeStep);
				isallterminated = allTerminated();
				fiveStepCounter++;

				// PRINTING //
				output(TimeStep);
				Sleep(1000);		//Wait for 1 second
				incrementTimeStep();
			}

		}

		else if (mode == 3)
		{
			pUI->printBeforeSim();
	
			bool isallterminated = allTerminated();		//simulation is working

			while (!isallterminated && isFileLoaded)
			{
				addtotermination();
				Simulation(TimeStep);
				isallterminated = allTerminated();
				fiveStepCounter++;

				incrementTimeStep();
			}
			pUI->printAfterSim();
		}
	}

	void Scheduler::LoadFile()
	{
		ifstream inputFile("input.txt");
		string line;   // lama ne3mel input file ha4el comment 
		if (inputFile.fail())
		{
			cout << "Cannot load input file!!" << endl;    // Check if file is not opened
			exit(1); // terminates program
		}
		else
		{
			isFileLoaded = true;
		}
		//------------------Line 1-----------------------------//
		while (getline(inputFile, line)) {
			inputFile >> NF;	// Number of FCFS processor
			inputFile >> NS;		// Number of SJF processor
			inputFile >> NR;		// Number of RR processor

			//------------------Line 2-----------------------------//
			// Skip the next line
			getline(inputFile, line);
			//------------------Line 3-----------------------------//
			// Skip the next line
			getline(inputFile, line);
		}
		//------------------Line 4-----------------------------//
		inputFile >> NumProcess;		// num of processes
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
