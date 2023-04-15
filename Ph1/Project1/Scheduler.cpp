#include "Scheduler.h"

	Scheduler::Scheduler()
	{
		pr = new RR;
		pf = new FCFS;
		ps = new SJF;
		pUI = new UI;
		ArrP = new Processor * [NF + NR + NS];
		isFileLoaded = false;
		NF = 0;
		NR = 0;
		NS = 0;
		TimeStep = 0;
		char type=0;
	}
	void Scheduler::Simulation(int currenttime)
	{
		// Seed the random number generator with the current time
		srand(currenttime);
		NumProcessor = NF + NS + NR;
		// create processors array
		for (int i = 0; i < NumProcessor; i++)
		{
			if (i < NF)
			{
				FCFS* fcfs = new FCFS();
				ArrP[i] = dynamic_cast<Processor*>(fcfs);
			}
			else if (i < NF + NS)
			{
				SJF* sjf = new SJF();
				ArrP[i] = dynamic_cast<Processor*>(sjf);
			}
			else
			{
				RR* rr = new RR();
				ArrP[i] = dynamic_cast<Processor*>(rr);
			}
		}
		// Use an array to keep track of the available processors
		Processor** availableProcessors = new Processor * [NumProcessor];
		int numAvailableProcessors = NumProcessor;
		for (int i = 0; i < NumProcessor; i++)
		{
			availableProcessors[i] = ArrP[i];
		}

		// Iterate through the new queue and randomly assign each process to a processor
		while (!NewQueue.isEmpty())
		{
			// Get the next process from the new queue
			Process process;
			NewQueue.dequeue(process);

			// Choose a random available processor to assign the process to
			int randomIndex = rand() % numAvailableProcessors;
			Processor* processor = availableProcessors[randomIndex];

			// Enqueue the process to the ready queue of the chosen processor
			if (FCFS* fcfs = dynamic_cast<FCFS*>(processor))
			{
				fcfs->addToReadyQueue(process);
			}
			else if (RR* rr = dynamic_cast<RR*>(processor))
			{
				rr->addToReadyQueue(process);
			}
			else if (SJF* sjf = dynamic_cast<SJF*>(processor))
			{
				sjf->addToReadyQueue(process);
			}

			// Remove the chosen processor from the available processors array
			availableProcessors[randomIndex] = availableProcessors[numAvailableProcessors - 1];
			numAvailableProcessors--;
		}
	}
	void Scheduler:: incrementTimeStep() 
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
		for (int i = 0; i < NS+NF+NR; i++)
		{
			if (ArrP[i]->getRDY_Length() > max->getRDY_Length())
				max = ArrP[i];
		}
		return max;
	}

	Processor* Scheduler::getMinProcessor()
	{
		Processor* min = ArrP[0];
		for (int i = 0; i < NS + NF + NR; i++)
		{
			if (ArrP[i]->getRDY_Length() < min->getRDY_Length())
				min = ArrP[i];
		}
		return min;
	}

	double Scheduler::StealLimit()
	{
		int shortest = getMinProcessor()->getRDY_Length();
		int longest = getMaxProcessor()->getRDY_Length();
		return (longest - shortest) / longest;
	}

	bool Scheduler::HandleBlk() 
	{}


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
					addtoBlock();
					isallterminated = allTerminated(); 
				// PRINTING //
				//output(TimeStep);
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
				addtoBlock();
				isallterminated = allTerminated();

				// PRINTING //
				//output(TimeStep);
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
				addtoBlock();
				isallterminated = allTerminated();

				incrementTimeStep();
			}
			pUI->printAfterSim();
		}
	}
/*	void Scheduler::output(int time)
	{
		pUI->Print1(TimeStep,NF, NS, NR, ReadyFCFS, ReadyRR, ReadySJF);
		pUI->Print2( BLKqueue, BLKcount);
		pUI->Print3( RUNqueue, runcount,  PID, type);
		pUI->Print4( TRMqueue, trmcount);
	}*/


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
		while (getline(inputFile, line))
		{
			inputFile >> NF;	// Number of FCFS processor
			inputFile >> NS;		// Number of SJF processor
			inputFile >> NR;		// Number of RR processor

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
			Pp = new Process;
			for (int i = 0; i < NumProcess; i++)
			{
			Process p =	Pp->load(inputFile);
			NewQueue.enqueue(p);
			}
		}
	}

	void Scheduler::addtotermination()
	{
		Process* Pts;
		Process* Ptr;
		Process* Ptf;
		if (ps)
		{
			ps->addtoterminate(Pts);
			if (Pts)
			{
				TerminatedQueue.enqueue(Pts);
			}
			else
			{
				return;
			}
		}
		if (pr)
		{
			pr->addtoterminate(Ptr);
			if (Ptr)
			{
				TerminatedQueue.enqueue(Ptr);
			}
			else
			{
				return;
			}
		}
		if (pf)
		{
			pf->addtoterminate(Ptf);
			if (Ptf)
			{
				TerminatedQueue.enqueue(Ptf);
			}
			else
			{
				return;
			}
		}
	}
	void Scheduler::addtoBlock()
	{
		Process* Pbs;
		Process* Pbr;
		Process* Pbf;
		if (ps)
		{
			ps->addtoBLK(Pbs);
			if (Pbs)
			{
				BLKQueue.enqueue(Pbs);
			}
			else
			{
				return;
			}
		}
		if (pr)
		{
			pr->addtoBLK(Pbr);
			if (Pbr)
			{
				BLKQueue.enqueue(Pbr);
			}
			else
			{
				return;
			}
		}
		if (pf)
		{
			pf->addtoBLK(Pbf);
			if (Pbf)
			{
				BLKQueue.enqueue(Pbf);
			}
			else
			{
				return;
			}
		}
	}
	Scheduler::~Scheduler()
	{
		for (int i = 0; i < NumProcessor; i++)
		{
			delete ArrP[i];
		}
		delete[] ArrP;
		if (pUI)
			delete pUI;
	}
