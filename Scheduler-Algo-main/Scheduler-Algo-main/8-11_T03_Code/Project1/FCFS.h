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
	static SQueue<int> killSig; //Kill Signal Time
	static char Ptype; // returning type instead of dynamic casting 
	LinkedList<Process*> RDY;
	static int MaxW; //to use for check in migration
	static double forkprob; //fork probability 
	static int nf; //holder to be incremented until number of FCFS processors, used to make sure all of them were checked in sigkill fn
	
public:
	FCFS();  //constructor
	void KillOrphan(Process* parent);
	void Handle(int timestep);
	virtual Process* sigkill(int timestep, int NF);
	virtual void ScheduleAlgo(int time);
	virtual int getRDYCount(); //number of processes in processor
	virtual void addToReadyQueue(Process* process); //inserting a process to the RDY 
	char getPtype();
	void print_rdy();
	static void Loadp(ifstream& inputFile); //loading MaxW
	static void Loadpf(ifstream& inputFile); //loading Forking prob
	static void Loadkill(ifstream& inputFile); // loading kill signals
	~FCFS();
};



