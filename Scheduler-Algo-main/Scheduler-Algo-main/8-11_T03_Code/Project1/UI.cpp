#include "UI.h"
#include<fstream>

UI::UI()
{
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
		cout << endl;
		cout << "Invalid Entry!!" << endl << "1. Interactive Mode" << endl << "2. Step-By-Step Mode" << endl << "3. Silent Mode" << endl << "Please choose a mode: ";
		cin >> mode;
	}
	return mode;
}


//Printing the Process Scheduler Status
void UI:: Print1 (int time, int NumProcessor, Processor ** ArrP , int NF, int NS, int NR, int ND )
{
	clear();
	cout << "Current Timestep:" << time << endl;
	cout << "--------------------RDY processes--------------------" << endl;	
		for (int i = 0; i < NumProcessor; i++)
		{
			if (ArrP[i] != NULL)
			{
				if (i < NF)//&& ArrP[i] != NULL
				{
					cout << "Processor " << i + 1 << "[FCFS]";
					cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";
					if (ArrP[i]->getisOverheated())
						cout << "FCFS Processor is over heated !" ;
					ArrP[i]->print_rdy();
					cout << endl;
				}
				else if (i < NF + NS )//&& ArrP[i] != NULL
				{
					cout << "Processor " << i + 1 << "[SJF]";
					cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";
					if (ArrP[i]->getisOverheated())
						cout << "SJF Processor is over heated !" ;
					
						ArrP[i]->print_rdy();
						cout << endl;
				}
				else if (i < NF+NS+NR)
				{
					cout << "Processor " << i + 1 << "[RR]";
					cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";
					if (ArrP[i]->getisOverheated())
					cout << "RR Processor is over heated !" ;
					
						ArrP[i]->print_rdy();
						cout << endl;
			
				}
				else
				{
					cout << "Processor " << i + 1 << "[EDF]";
					cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";
					if (ArrP[i]->getisOverheated())
						cout << " EDF Processor is over heated !";
					ArrP[i]->print_rdy();
					cout << endl;
				}
			}
		}
}
void UI::Print2(Queue<Process*> BLKqueue, int BLKcount)
{
	cout << "--------------------BLK processes--------------------" << endl;
	cout << BLKcount << " BLK: ";
	BLKqueue.Print();
	
	
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
	cout << count << " RUN: ";
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getCurrRun())
		cout << ArrP[i] ->getCurrRun()->getPID() << "(P"<<i+1<<") ";
	}
	cout << endl;
}
void UI::Print4(Queue<Process*> TRMqueue, int trmcount )
{
	cout << "--------------------TRM processes--------------------" << endl;
	cout << trmcount << " TRM: ";
	TRMqueue.Print();
	
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
