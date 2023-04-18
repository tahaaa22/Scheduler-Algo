#include "Scheduler.h"

	Scheduler::Scheduler()
	{
		pr = new RR;
		pf = new FCFS;
		ps = new SJF;
		pUI = new UI;
		ArrP = new Processor * [NF + NR + NS];
		isFileLoaded = true;
		NF = 0;
		NR = 0;
		NS = 0;
		TimeStep = 0;
		char type=0;
	}
	void Scheduler::Simulation()
	{
		LoadFile();
		NumProcessor = NF + NS + NR;
		full();
		int mode;
		mode = pUI->ReadMode();
		cin.ignore();  //Clear any leftover characters in the input buffer
		// create processors array
		
		
		if (mode == 1)
		{
			bool isallterminated = allTerminated();	//simulation is working // btba b 0 fl awl
			
		
			while (!isallterminated && isFileLoaded)
			{
				Simulation(TimeStep);
				addtoBlock();
				HandleBlk(TimeStep);
				isallterminated = allTerminated();
				// PRINTING //
				Output(TimeStep);
				getchar();	// Waits for user to press "Enter" 
				incrementTimeStep();
				
				
			}
		}//exit of while loop(program)
		else if (mode == 2)
		{
			bool isallterminated = allTerminated();	//simulation is working

			while (!isallterminated && isFileLoaded)
			{
				Simulation(TimeStep);
				addtoBlock();
				HandleBlk(TimeStep);
				isallterminated = allTerminated();

				// PRINTING //
				Output(TimeStep);
				Sleep(25);		//Wait for 1 second
				incrementTimeStep();
				
			}

		}

		else if (mode == 3)
		{
			pUI->printBeforeSim();

			bool isallterminated = allTerminated();		//simulation is working

			while (!isallterminated && isFileLoaded)
			{
				Simulation(TimeStep);
				addtoBlock();
				isallterminated = allTerminated();

				incrementTimeStep();
			}
			pUI->printAfterSim();
		}
	}
	void Scheduler::full()
	{
		for (int i = 0; i < NumProcessor; i++)
		{
			if (i < NF)
			{
				FCFS* fcfs = new FCFS();
				ArrP[i] = fcfs;
			}
			else if (i < NF + NS)
			{
				SJF* sjf = new SJF();
				ArrP[i] = sjf;

			}
			else
			{
				RR* rr = new RR();
				ArrP[i] = rr;

			}
		}
	}
	void Scheduler::Simulation(int currenttime)
	{
		// Seed the random number generator with the current time
		srand(currenttime);
		int random = rand() % 100;
		// Iterate through the new queue and randomly assign each process to a processor
		Process* process;
		NewQueue. peek(process);
		while (!NewQueue.isEmpty() && process->getArrivalTime() == currenttime)
		{
			
			// Get the next process from the new queue
			// Choose a random available processor to assign the process to
			int randomIndex = rand() % NumProcessor ;
			if (randomIndex >= 0 || randomIndex < NumProcessor)
			{
				Processor* processor = ArrP[randomIndex];
				// Enqueue the process to the ready queue of the chosen processor
				if (process->getArrivalTime() == currenttime)
				{
					processor->addToReadyQueue(process);
					NewQueue.dequeue(process);

				}
				NewQueue.peek(process);
			}
		}
			// Choose a random available processor to assign the process to
		Process* Pts = NULL;
		Process* ts = NULL;
		for (int i = 0; i < NumProcessor; i++)
		{
			Processor* proc = ArrP[i];
			if (proc)
			proc->ScheduleAlgo(random, NumProcess);
			Pts = ArrP[i]->getTerminated();
			ts = ArrP[i]->getkilltem();
			if (Pts != NULL && ts != NULL)
			{
				TerminatedQueue.enqueue(ts);
				ArrP[i]->addterminate(NULL);
				TerminatedQueue.enqueue(Pts);
				ArrP[i]->addtoterminate(NULL);
			}
			else if (Pts != NULL)
			{
				TerminatedQueue.enqueue(Pts);
				ArrP[i]->addtoterminate(NULL);
			}
			else if(ts != NULL)
			{
				TerminatedQueue.enqueue(ts);
				ArrP[i]->addterminate(NULL);
			}
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
	void  Scheduler::HandleBlk(int currenttime) 
	{
		
			if (!BLKQueue.isEmpty())
			{
				srand(currenttime);
				int rad = rand() % NumProcessor;
				int random = rand() % 100;
				if (random <= 10)
				{
					Process* t;
					BLKQueue.dequeue(t);
					ArrP[rad]->addToReadyQueue(t);
				}

			}
	}
	void Scheduler::Output(int time)
	{
		pUI->Print1(TimeStep,NumProcessor,ArrP , NF, NS , NR);
		pUI->Print2(BLKQueue, BLKQueue.getCount());
		pUI-> Print3(NumProcessor, ArrP);
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
			

			//------------------Line 2-----------------------------//
			// Skip the next line
			pr-> Loadp(inputFile);
			//------------------Line 3-----------------------------//
			pf-> Loadp(inputFile);
			inputFile >> STL;
			
			pf-> Loadpf(inputFile);

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
		
	}

	
	void Scheduler::addtoBlock()
	{
		Process* Pbs = NULL;
		
		for (int i = 0; i < NumProcessor; i++)
		{
			Pbs = ArrP[i]->getBLK();
			if (Pbs != NULL)
			{
				BLKQueue.enqueue(Pbs);
				ArrP[i]->addtoBLK(NULL);
			}
		}
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
