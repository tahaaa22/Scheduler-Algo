#include "UI.h"
#include<fstream>

UI::UI()
{
	pf = new FCFS;
	ps = new SJF;
	pr= new RR;
}

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
void UI:: Print1 (int time, int NumProcessor, Processor ** ArrP , int NF, int NS, int NR )
{
	clear();
	cout << "Current Timestep:" << time << endl;
	cout << "--------------------RDY processes--------------------" << endl;	
		for (int i = 0; i < NumProcessor; i++)
		{
			if (i < NF)
			{
				cout << "Processor " << i + 1 << "[FCFS]";
				pf->ReadyIDs();
				cout << endl;
			}
			else if (i < NF + NS)
			{
				cout << "Processor " << i + 1 << "[SJF]";
				ps->ReadyIDs();
				cout << endl;
			}
			else
			{
				cout << "Processor " << i + 1 << "[RR]";
				pr->ReadyIDs();
				cout << endl;
			}
		}
	/*cout << "Current Timestep:" << time << endl;
	cout << "--------------------RDY processes--------------------" << endl;
	cout << "processor 1 [FCFS]:" << NF << "RDY: ";
	pf->print_rdy();
	cout << ", " << endl;
	cout << "processor 2 [SJF]:" << NS << "RDY: " ;
	ps->print_rdy();
	cout << ", " << endl;
	cout << "processor 3 [RR]:" << NR << "RDY: ";
	pr->print_rdy();
	cout << ", " << endl;*/
}
void UI::Print2(Queue<Process*> BLKqueue, int BLKcount)
{
	cout << "--------------------BLK processes--------------------" << endl;
	cout << BLKcount << "BLK: ";
	BLKqueue.Print();
	cout << ", ";
	
}
void UI::Print3(int NumProcessor, Processor** ArrP)
{
	int count = 0;
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getCurrRun())
		{
			count++;
		}
	}
	cout << endl;
	cout << "--------------------RUN processes--------------------" << endl;
	cout << count << "RUN: ";
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getCurrRun())
		cout << ArrP[i] ->getCurrRun()->getPID()<< "(P"<<i+1<<"), ";
	}
	cout << endl;
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
