#include "UI.h"
#include<fstream>

UI::UI()
{
}

void UI::clear()
{
	system("CLS"); //clears the console 
}

int UI::ReadMode()  // reads the required mode 1-> interactive 2-> step by step 3-> silent 
{
	int mode;
	cout << "1. Interactive Mode" << endl << "2. Step - By - Step Mode" << endl << "3. Silent Mode" << endl << "Please choose a mode: ";
	cin >> mode;  // takes the input
	while (mode != 1 && mode != 2 && mode != 3)  // validates input is only 1, 2 or 3
	{
		clear();
		cout << endl;
		cout << "Invalid Entry!!" << endl << "1. Interactive Mode" << endl << "2. Step-By-Step Mode" << endl << "3. Silent Mode" << endl << "Please choose a mode: ";
		cin >> mode;
	}
	return mode;
}



void UI::printwelcome()  //Printing the Process Scheduler Status (elegant but not required credit to Ahmed Taha)
{
	string message = "WELCOME TO OUR PROJECT <3";

	// get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// calculate message position
	int messageWidth = message.length();
	int messageHeight = 1;
	int x = (width - messageWidth) / 2;
	int y = (height - messageHeight) / 2;

	// set cursor position
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	// print message
	for (char c : message)
	{
		cout << c;
		Sleep(100);
	}
	Sleep(1000);
	clear();

}
void UI::printwarning()  // message printed in case no processors are in the input file 
{
	string message = "No processors in the system to start please make sure you entered processors.";

	// get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// calculate message position
	int messageWidth = message.length();
	int messageHeight = 1;
	int x = (width - messageWidth) / 2;
	int y = (height - messageHeight) / 2;

	// set cursor position
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	// print message
	for (char c : message)
	{
		cout << c;
		Sleep(100);
	}
	Sleep(1000);

}
void UI::Print1(int time, int NumProcessor, Processor** ArrP, int NF, int NS, int NR, int ND)  // prints the ready lists
{
	//clear();
	cout << "Current Timestep:" << time << endl;  // displays the current time step
	cout << "--------------------RDY processes--------------------" << endl;
	for (int i = 0; i < NumProcessor; i++)  // keep in mind, the array of processor pointers is sorted such that all FCFS processors come first then SJF then RR then EDF
	{
		if (ArrP[i] != NULL)
		{
			if (i < NF)
			{
				cout << "Processor " << i + 1 << "[FCFS]";
				cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";  // displays number of processes in the ready list 
				if (ArrP[i]->getisOverheated())
					cout << "FCFS Processor is overheated !";  // display message if the processor is overheated 
				ArrP[i]->print_rdy();
				cout << endl;
			}
			else if (i < NF + NS)
			{
				cout << "Processor " << i + 1 << "[SJF]";
				cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";  // displays number of processes in the ready list 
				if (ArrP[i]->getisOverheated())
					cout << "SJF Processor is overheated !";  // display message if the processor is overheated 

				ArrP[i]->print_rdy();
				cout << endl;
			}
			else if (i < NF + NS + NR)
			{
				cout << "Processor " << i + 1 << "[RR]";
				cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";  // displays number of processes in the ready list 
				if (ArrP[i]->getisOverheated())
					cout << "RR Processor is overheated !";  // display message if the processor is overheated 

				ArrP[i]->print_rdy();
				cout << endl;

			}
			else
			{
				cout << "Processor " << i + 1 << "[EDF]";
				cout << " " << ArrP[i]->getRDYCount() << " " << "RDY: ";  // displays number of processes in the ready list 
				if (ArrP[i]->getisOverheated())
					cout << " EDF Processor is overheated !";  // display message if the processor is overheated 
				ArrP[i]->print_rdy();
				cout << endl;
			}
		}
	}
}
void UI::Print2(Queue<Process*> BLKqueue, int BLKcount)  // prints the blocked list 
{
	cout << "--------------------BLK processes--------------------" << endl;
	cout << BLKcount << " BLK: ";  // displays the number of processes in the blocked list 
	BLKqueue.Print();


}
void UI::Print3(int NumProcessor, Processor** ArrP)  // prints the run states 
{
	int count = 0;
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getCurrRun())
		{
			count++;  // returns the number of processors with a process in the run at that time step 
		}
	}

	cout << endl;
	cout << "--------------------RUN processes--------------------" << endl;
	cout << count << " RUN: ";  // displays the number of processors with a process in the run at that time step 
	for (int i = 0; i < NumProcessor; i++)
	{
		if (ArrP[i]->getCurrRun())
			cout << ArrP[i]->getCurrRun()->getPID() << "(P" << i + 1 << ") ";  // displays the run states of processors with a process in the run at that time step 
	}
	cout << endl;
}
void UI::Print4(Queue<Process*> TRMqueue, int trmcount)  // prints the terminate list
{
	cout << "--------------------TRM processes--------------------" << endl;
	cout << trmcount << " TRM: ";  // displays the number of terminated processes 
	TRMqueue.Print();

}
void UI::printending()  //Printing the Process Scheduler Status (elegant but not required credit to Ahmed Taha)
{
	clear();
	string message = "Simulation endedddd :) \n\n\t\t\t\t\t\tTHANK YOU <3";
	string message2 = "THANK YOU <3 ";
	// get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// calculate message 1 position
	int messageWidth = message.length();
	int messageHeight = 1;
	int x1 = (width - messageWidth) / 2;
	int y1 = (height - messageHeight) / 2;
	//// calculate message 2 position
	//int messageWidth2 = message2.length();
	//int messageHeight2 = 1;
	//int x2 = (width - messageWidth2) / 2;
	//int y2 = (height - messageHeight2) / 2;

	// set cursor position for message 1
	COORD pos1 = { x1, y1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos1);

	//// set cursor position for message 2
	//COORD pos2 = { x2, y2 };
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos2);
	// print message
	for (char c : message)
	{
		cout << c;
		Sleep(100);
	}
	Sleep(1000);
	clear();
	//for (char c : message2)
	//{
	//	cout << c;
	//	Sleep(100);
	//}


}

void UI::printBeforeSim()  // prints the silent mode message 
{
	clear();
	cout << "Silent Mode" << endl << "Simulation Starts..." << endl;
}

void UI::printAfterSim()  // prints message that output file is created 
{
	cout << "Simulation ends, Output file created" << endl;
}

UI::~UI()
{}