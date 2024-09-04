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
	int ReadMode();  // reads the mode wanted (interactive, step by step or sleep mode)

	// Printing the Process Scheduler Status
	void printwelcome(); //welcoming msg to start the system
	void printwarning(); //warning msg for user if number of processors is zero 
	void Print1(int time, int NumProcessor, Processor** ArrP, int NF, int NS, int NR, int ND);  // prints the ready lists 
	void Print2(Queue<Process*> BLKqueue, int BLKcount);  // prints the blocked list
	void Print3(int NumProcessor, Processor** ArrP);  // prints the run 
	void Print4(Queue<Process*>  TRMqueue, int trmcount );  // prints the terminate list
	void printending(); // ending msg when simulation is ended
	// Silent Mode
	void printBeforeSim();
	void printAfterSim();

};
