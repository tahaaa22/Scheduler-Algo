#pragma once
#include <iostream>
#include <string>
#include "Queue.h"
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
	// void Print(); // print function will be repeated for each line in console screen 
	
	// Silent Mode
	void printBeforeSim();
	void printAfterSim();

};