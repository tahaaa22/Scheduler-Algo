#pragma once
#include <iostream>
#include <string>
#include "Queue.h"
#include <string>
#include <fstream>
#include "LinkedList.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include <conio.h>
#include "Process.h"
#include "Processor.h"
using namespace std;

class UI
{
public:
	void clear(); // to clear console after each time step
	UI();
	~UI();

	// Reading the required mode
	int ReadMode();

	// Printing the Process Scheduler Status
	void Print1(int time, int NF, int NS, int NR, LinkedList<Process*>& ReadyFCFS, Queue<Process*>& ReadyRR, PriorityQueue<Process*>& ReadySJF);
	void Print2(Queue<Process*> BLKqueue, int BLKcount);
	void Print3(Queue<Process*> RUNqueue, int runcount, int ID, char type);
	void Print4(Queue<Process*> TRMqueue, int trmcount);
	// Silent Mode
	void printBeforeSim();
	void printAfterSim();

};
