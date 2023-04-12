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

	bool Scheduler::TerminateAll() {

		//if(TerminatedQueue.getCount()== NumProcess)
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
 // create an array of queues, where the index corresponds to the process ID
   /* const int MAX_PROCESS_ID = 100;    //waiting for TA to ans -taha
    array<SQueue<int>*, MAX_PROCESS_ID> processQueues{};

    // open the input file
    ifstream inputFile("inputfile.txt");

    if (inputFile.is_open()) {
        int processId, value1, value2;

        // read each line of the input file
        while (inputFile >> processId >> value1 >> value2) 
        {
            // create a new queue for the process if it doesn't already exist
            if (processQueues[processId] == nullptr) {
                processQueues[processId] = new SQueue<int>();
            }

            // enqueue the values in the queue for the process
            SNode<int>* node = new SNode<int>(value1, value2);
            processQueues[processId]->enqueue(node);
        }

        // close the input file
        inputFile.close();
    }
    else {
        std::cout << "Unable to open file!" << std::endl;
        return 1;
    }

    // print the contents of the queues for each process
    for (int i = 0; i < MAX_PROCESS_ID; i++) {
        if (processQueues[i] != nullptr) {
            std::cout << "Process " << i << ": ";

            while (!processQueues[i]->isEmpty()) {
                SNode<int> dequeuedNode = processQueues[i]->dequeue();
                cout << dequeuedNode.getFirstItem() << ", " << dequeuedNode.getSecondItem() << " ";
            }

            cout << std::endl;

            // clean up memory for the queue
            delete processQueues[i];
        }
    }*/

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
