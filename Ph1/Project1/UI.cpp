#include "UI.h"
#include<fstream>
UI::UI()
{}

void UI::clear()
{
	system("CLS"); //clear console 
}

//Reading the required mode
int UI::ReadMode()
{
	int mode;
	cout << "1. Interactive Mode" << endl << "2. Step - By - Step Mode" << endl << "3. Silent Mode" << endl << "Please choose a mode: ";
	cin >> mode;
	while (mode != 1 && mode != 2 && mode != 3)
	{
		clear();
		cout << "Invalid Entry!!" << endl << "1. Interactive Mode" << endl << "2. Step-By-Step Mode" << endl << "3. Silent Mode" << endl << "Please choose a mode: ";
		cin >> mode;
	}
	return mode;
}

//Printing the Process Scheduler Status
 /*void UI::Print()
{}*/

// Silent Mode
void UI::printBeforeSim()
{
	clear();
	cout << "Silent Mode" << endl << "Simulation Starts..." << endl;
}

void UI::printAfterSim()
{
	cout << "Simulation ends, Output file created" << endl;
}

UI::~UI()
{}