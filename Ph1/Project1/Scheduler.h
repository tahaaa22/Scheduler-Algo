#pragma once
#include "Processor.h"
#include "Process.h"
#include "UI.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "LinkedList.h"
#include "binarySearchTree.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include "SJF.h"
using namespace std;
class Scheduler
{
	SJF* ps;
	UI* pUI;
	int NR,NF ,NS , RRtime, RTF, MaxW, STL, NumProcess, AT ;
	double Pfork;
	int fiveStepCounter;
	bool isFileLoaded; // File load boolean
	Queue <Process*> NewQueue;
	Queue <Process*> TerminatedQueue;
	Queue <Process*> BLKQueue;
public :
	
	Scheduler();
	void Mode(); // read modes from UI class to make different implementation for each mode
	bool allTerminated(); // check if the terminated queue has equal number of processes entered to stop simulation 
	void Simulation(int currenttime);
	void AddReady(Process* p);	// Add Processes to the suitable queue
	void addtotermination();
	void addNewProcess(Process* process)     // add process to new queue
	{
		NewQueue.enqueue(process);
	}
	void addtoBlock();
	
	//	INPUT-OUTPUT functions
	//void Output(int currenttime); // link between print functions from UI class and scheduler class so we maintain classes responsibility
	void LoadFile(); // load inputs for memebers 
	~Scheduler();
};
