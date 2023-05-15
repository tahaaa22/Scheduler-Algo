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
#include <Windows.h>
#include <iomanip> // for setw()
#include <chrono> // for sleep_for()
#include <thread> // for sleep_for()
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
	void printwelcome();
	void Print1(int time, int NumProcessor, Processor** ArrP, int NF, int NS, int NR, int ND);
	void Print2(Queue<Process*> BLKqueue, int BLKcount);
	void Print3(int NumProcessor, Processor** ArrP);
	void Print4(Queue<Process*>  TRMqueue, int trmcount );
	void printending();
	// Silent Mode
	void printBeforeSim();
	void printAfterSim();

};
