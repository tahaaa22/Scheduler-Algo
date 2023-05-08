#pragma once
#include <fstream>
#include <iostream>
#include <ostream>
#include "Processor.h"
#include "LinkedList.h"
#include "StructQueue.h"
#include "Node.h"
using namespace std;
//class Scheduler;

class FCFS : public Processor
{
	//The FCFS is a queue but because of the kill signal that will require accessing 
	// a process in the middle of the queue the structure is now a LinkedList
private:
	int TotalBusyTime; // time taken inside run for each processor
	int TotalIdleTime; // total busy time minus time step
	int TotalTRT; // total turn around time of all processes
	static SQueue<int> killSig; //Kill Signal Time
	static char Ptype; // returning type instead of dynamic casting 
	LinkedList<Process*> RDY;
	static int MaxW; //to use for check in migration
	static double forkprob; //fork probability 
	static int nf; //holder to be incremented until number of FCFS processors, used to make sure all of them were checked in sigkill fn
	//Scheduler* s; //used to call fork fn

public:
	FCFS(Scheduler *);  //constructor

	virtual Process* gettop() ;

	void sigkill(int timestep, int NF);

	virtual double pLoad();

	virtual double pUtil();

	virtual void ScheduleAlgo(int time);

	void KillOrphan(Process* parent, int time);

	void Handle(int timestep);

	virtual int getRDYCount(); //number of processes in processor

	virtual void addToReadyQueue(Process* process); //inserting a process to the RDY 

	char getPtype();

	void print_rdy();

	static void Loadp(ifstream& inputFile); //loading MaxW

	static void Loadpf(ifstream& inputFile); //loading Forking prob

	static void Loadkill(ifstream& inputFile); // loading kill signals

	Process* eject(); //added by omar

	~FCFS();
};



