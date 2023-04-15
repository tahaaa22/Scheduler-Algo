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
void UI:: Print1 (int time,int NF,int NS, int NR , LinkedList<Process*>& ReadyFCFS, Queue<Process*>& ReadyRR, PriorityQueue<Process*>& ReadySJF)
{
	clear();
	cout << "Current Timestep:" << time << endl;
	cout << "--------------------RDY processes--------------------" << endl;
	cout << "processor 1 [FCFS]:" << NF << "RDY: ";
	ReadyFCFS.Print();
	cout << ", " << endl;
	cout << "processor 2 [SJF]:" << NS << "RDY: " ;
	ReadySJF.Print();
	cout << ", " << endl;
	cout << "processor 3 [RR]:" << NR << "RDY: ";
	ReadyRR.Print();
	cout << ", " << endl;
}
void UI::Print2(Queue<Process*> BLKqueue, int BLKcount)
{
	cout << "--------------------BLK processes--------------------" << endl;
	cout << BLKcount << "BLK: ";
	BLKqueue.Print();
	cout << ", ";
}
void UI::Print3(Queue<Process*> RUNqueue, int runcount, int ID, char type)
{
	cout << "--------------------RUN processes--------------------" << endl;
	cout << runcount << "RUN: ";
	cout << ID<<"(";
	cout << type << "), ";
}
void UI::Print4(Queue<Process*> TRMqueue, int trmcount)
{
	cout << "--------------------TRM processes--------------------" << endl;
	cout << trmcount << "TRM: ";
	TRMqueue.Print();
	cout << ", ";
}
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
