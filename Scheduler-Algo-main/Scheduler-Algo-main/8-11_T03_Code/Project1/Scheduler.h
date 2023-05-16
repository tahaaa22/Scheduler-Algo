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
	Processor ** ArrP;  // array of pointers to processors 
	int Blkcounter; // counter to know the needed time to dequeue from blk queue
	RR* pr;  // pointer to RR
	FCFS* pf;  // pointer to FCFS
	SJF* ps;  // pointer to SJF
	EDF* pd;  // pointer to EDF
	UI* pUI;  // pointer to the UI
	Process* Pp;  // pointer to process used during load
	int OverheatConstant;  // number of timesteps the processor overheats for 
	int TimeStep;  
	bool minflag;  // used in migration & stealing
	bool maxflag;  // used in migration & stealing
	int NumProcessor;  // total number of processors 
	double pKill = 0;
	double pFork, Migrate, pSteal, Avg_Util, Avg_WT, Avg_RT, Avg_TRT, pMaxW, pRTF;  //percentages
	int Total_WT, Total_RT, Total_TRT;
	int NR,NF ,NS,ND, STL, NumProcess, AT ;
	bool isFileLoaded; // File load Boolean
	Queue <Process*> NewQueue;  // new list
	Queue <Process*> TerminatedQueue;  // terminate list
	Queue <Process*> BLKQueue;  // blocked list 
	string ofname; // output file name
public :
	Scheduler();
	void full();
	void incrementTimeStep();
	//double StealLimit();
	void Steal();
	void Overheat(Processor* p);
	//////////////////////Added by mimo//////////////////////////
	bool migrationrtf(Process* p, int rtf);
	bool migrationmaxw(Process* p, int maxw, int timestep);
	////////////////////////////////////////////////////////////
	void  BlktoRdy();
	void Simulation(); // read modes from UI class to make different implementation for each mode
	bool allTerminated(); // check if the terminated queue has equal number of processes entered to stop simulation 
	void NewtoRdy(int timestep);
	void Trm(Process* p);
	void RuntoBlk(Process* p);
	void AddtoRdy(Process* temp);
	FCFS* GetFCFSprocessor(int n);
	~Scheduler();

	/////////////////////////////////////////
	//		INPUT-OUTPUT functions         //
	/////////////////////////////////////////

	void Output(int currenttime); // link between print functions from UI class and scheduler class so we maintain classes responsibility
	void LoadFile(); // load inputs for members 
	void OutputFile();
	//////////////////////Added by Amira//////////////////////////
	void fork(Process* parent);
	void Kill(Process* orphan); //search for orphan and kill

	/////////////////////////////////////////
	//				Getters				   //
	/////////////////////////////////////////
	int getTimeStep();
	int getNF();
	double getpKill();
	Processor* getMaxProcessor();  // gets the processor with longest ready list
	Processor* getMinProcessor(char a, int n);  // gets the processor with shortest ready list of the type we choose 

	/////////////////////////////////////////
	//				setters				   //
	/////////////////////////////////////////
	void setpKill(int n);
};
