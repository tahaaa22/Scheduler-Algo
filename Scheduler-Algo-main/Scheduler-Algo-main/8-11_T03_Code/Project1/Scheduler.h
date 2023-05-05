#pragma once
#include "Process.h"
#include "UI.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "LinkedList.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include "SJF.h"
#include "FCFS.h"
#include "RR.h"
#include "Processor.h"
#include "EDF.h"
#include <Windows.h>
#include<string>
using namespace std;
class Scheduler
{
	Processor ** ArrP;
	RR* pr;
	FCFS* pf;
	SJF* ps;
	EDF* pd;
	UI* pUI;
	Process* Pp;
	int ID_Count;     //to be initialized as the number of processes from the input file in the constructor
	int TimeStep; 
	int NumProcessor;
	double Kill, Fork, Migrate, Stealp, Avg_Util, Avg_WT, Avg_RT, Avg_TRT, pMaxW, pRTF;  //percentages
	int Total_WT, Total_RT, Total_TRT;
	int NR,NF ,NS,ND, STL, NumProcess, AT ;
	bool isFileLoaded; // File load Boolean
	Queue <Process*> NewQueue;
	Queue <Process*> TerminatedQueue;
	Queue <Process*> BLKQueue;
public :
	void full();
	Scheduler();
	void incrementTimeStep();
	int getTimeStep();
	int getNF();
	double StealLimit();
	bool Steal(int STL);
	void MigrateToSJF();
	void MigrateToRR();
	Processor* getMaxProcessor();
	Processor* getMinProcessor(char a, int n); //true gets min of all types false gets min fcfs
	void  BlktoRdy();
	void Simulation(); // read modes from UI class to make different implementation for each mode
	bool allTerminated(); // check if the terminated queue has equal number of processes entered to stop simulation 
	void Simulation(int currenttime);
	//void addtotermination();
	void addtoBlock();
	//	INPUT-OUTPUT functions
	void Output(int currenttime); // link between print functions from UI class and scheduler class so we maintain classes responsibility
	void LoadFile(); // load inputs for members 
	void NewtoRdy(int timestep);
	void RuntoTrm(Process* p);
	void RuntoBlk(Process* p);
	void AddtoRdy(Process* temp);
	//////////////////////Added by Amira//////////////////////////
	void fork(Process* parent);
	//////////////////////////////////////////////////////////
	~Scheduler();
	//////////////////////Added by mimo//////////////////////////
	bool migrationrtf(Process* p, int rtf);
	bool migrationmaxw(Process* p, int maxw, int timestep);
};
