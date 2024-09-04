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
	LinkedList<Process*> RDY; //RDY list
	static int MaxW; //to use for check in migration
	static double forkprob; //fork probability 
	static int nf; //holder to be incremented until number of FCFS processors, used to make sure all of them were checked in sigkill fn

public:
	FCFS(Scheduler*); //constructor

	bool sigkill(int timestep, int NF); //Kill Signal

	void KillOrphan(Process* parent, int time); //Killing children and grandchildren

	virtual char getPtype(); //getter for the processor type (represented as a character)

	virtual void print_rdy(); //printing the rdy list

	void KillRun(Process* orphan); //killig a process in RUN state

	static void Loadp(ifstream& inputFile); //loading MaxW

	static void Loadpf(ifstream& inputFile); //loading Forking prob

	static void Loadkill(ifstream& inputFile); // loading kill signals

	bool isfound(Process* p); //checks if orphan is in processor's RDY list

	void deleteNodeK(Process* p); //delete and adjust the rdy length

	~FCFS(); //destructor

	/////////////////////////////////////////
	//		    Virtual Functions	       //
	/////////////////////////////////////////
	virtual Process* gettop();

	virtual Process* eject();

	virtual void Handle(int timestep);

	virtual void OverheatRun(int overheatconst);

	virtual double pUtil();

	virtual void ScheduleAlgo(int time);

	virtual int getRDYCount(); //number of processes in processor

	virtual void addToReadyQueue(Process* process, int time); //inserting a process to the RDY 

};