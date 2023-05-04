#pragma once
#include <iostream>
#include <string>
#include "Queue.h"
#include <fstream>
#include "LinkedList.h"
#include "PriorityQueue.h"
#include <conio.h>
#include "Process.h"
#include"FCFS.h"
#include "RR.h"
#include "SJF.h"
using namespace std;

class UI
{
private:
	RR* pr;
	FCFS* pf;
	SJF* ps;
public:
	
	void clear(); // to clear console after each time step
	UI();
	~UI();

	// Reading the required mode
	int ReadMode();

	// Printing the Process Scheduler Status
	void Print1(int time, int NumProcessor, Processor** ArrP, int NF, int NS, int NR);
	void Print2(Queue<Process*> BLKqueue, int BLKcount);
	void Print3(int NumProcessor, Processor** ArrP);
	void Print4(Queue<Process*>  TRMqueue, int trmcount );
	// Silent Mode
	void printBeforeSim();
	void printAfterSim();

};
