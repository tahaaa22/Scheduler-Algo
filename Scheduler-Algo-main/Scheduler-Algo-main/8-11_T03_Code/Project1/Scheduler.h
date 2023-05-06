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
	double pKill = 0;
	double pFork, Migrate, pSteal, Avg_Util, Avg_WT, Avg_RT, Avg_TRT, pMaxW, pRTF;  //percentages
	int Total_WT, Total_RT, Total_TRT;
	int NR,NF ,NS,ND, STL, NumProcess, AT ;
	bool isFileLoaded; // File load Boolean
	Queue <Process*> NewQueue;
	Queue <Process*> TerminatedQueue;
	Queue <Process*> BLKQueue;
	string ofname; // output file name
public :
	Scheduler();
	void full();
	void incrementTimeStep();
	//double StealLimit();
	void Steal();
	void MigrateToSJF();
	void MigrateToRR();
	void  BlktoRdy();
	void Simulation(); // read modes from UI class to make different implementation for each mode
	bool allTerminated(); // check if the terminated queue has equal number of processes entered to stop simulation 
	//void addtoBlock();
	void NewtoRdy(int timestep);
	void Trm(Process* p);
	void RuntoBlk(Process* p);
	void AddtoRdy(Process* temp);
	~Scheduler();
	/////////////////////////////////////////
	//		INPUT-OUTPUT functions         //
	/////////////////////////////////////////
	void Output(int currenttime); // link between print functions from UI class and scheduler class so we maintain classes responsibility
	void LoadFile(); // load inputs for members 
	void OutputFile();
	//////////////////////Added by Amira//////////////////////////
	void fork(Process* parent);
	//////////////////////////////////////////////////////////
	
	//////////////////////Added by mimo//////////////////////////
	bool migrationrtf(Process* p, int rtf);
	bool migrationmaxw(Process* p, int maxw, int timestep);

	/////////////////////////////////////////
	//				Getters				   //
	/////////////////////////////////////////
	int getTimeStep();
	int getNF();
	double getpKill();
	Processor* getMaxProcessor();
	Processor* getMinProcessor(char a, int n); //true gets min of all types false gets min fcfs
	/////////////////////////////////////////
	//				setters				   //
	/////////////////////////////////////////
	void setpKill(int n);




};
