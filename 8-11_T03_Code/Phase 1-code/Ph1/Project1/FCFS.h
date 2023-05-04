#pragma once
#include <fstream>
#include <iostream>
#include <ostream>
#include "Processor.h"
#include "LinkedList.h"
#include "StructQueue.h"
#include "Node.h"
using namespace std;

class FCFS : public Processor
{
	//The FCFS is a queue but because of the kill signal that will require accessing 
	// a process in the middle of the queue the structure is now a LinkedList
private:
	static SQueue<int> killSig; //Kill Signal Time
	static char Ptype; // returning type instead of dynamic casting 
	LinkedList<Process*> RDY;
	double fork; //fork probability
	static int MaxW; //to use for check in migration
	static double forkprob;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////MARIAMMMMM////////////////////////////////////////////////////////////////////////////////////////
	static int nf;

public:
	FCFS ( int m=0, double f=0)
	{
		settype('f');
	}


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////MARIAMMMMM////////////////////////////////////////////////////////////////////////////////////////
	void sigkill(int timestep, int NF)
	{
		SNode<int> Q;
		Q=killSig.peek();
		int kill=Q.getFirstItem(); // get the kill time
		int ID = Q.getSecondItem(); // get id
		Node<Process*>* temp = RDY.getHead(); // get head
		if (timestep == kill)
		{
			nf++;
			//check lw el process dy 3ndy hena 
			while (temp) //traversing the RDY list until I found the process with that ID
			{
				if (temp->getItem()->getPID() == ID)
				{
					Process* p = temp->getItem();
					addterminate(p); //terminate this process
					RDY.deleteNode(p); //delete it from RDY List
					break;
				}
				temp = temp->getNext();
			}
			if (getCurrRun())
			{
				int IDR = getCurrRun()->getPID();
				if (IDR == ID)
				{
					addterminate(getCurrRun()); //terminate this process
					setCurrRun(nullptr);// RUN CURRENTLY EMPTY
				}

			}
			if (nf == NF)
			{
				killSig.dequeue();
				nf = 0; 
			}
		}


	}

	
	
	virtual void ScheduleAlgo(int time , int Num)
	{
		
		if (!RDY.isEmpty() && !getCurrRun()) //run empty and ready contains processes
		{
			Process* temp = RDY.getHead()->getItem(); //First Process In and the turn is on this Process to RUN
			RDY.deleteNode(); //deleting first Process as it is removed to RUN 
			setCurrRun(temp); //setting the RUN state to this 
			
		}
		else if (getCurrRun())  //run not empty
		{
			if (time <= 15)
			{
				Process* P = getCurrRun();
				getCurrRun()->setisBlocked(true);
				addtoBLK(P);
				setCurrRun(nullptr);
			}
			else if (time >= 20 && time <= 30)
			{
				addToReadyQueue(getCurrRun());
				setCurrRun(nullptr);
			}
			else if (time >= 50 && time <= 60)
			{
				Process* P = getCurrRun();
				addtoterminate(P);
				setCurrRun(nullptr);
			}
		}
	}

	virtual int getRDYCount()
	{
		return RDY.getCount();
	}

	void  Check_Kill(int timestep) {} //will be added in phase 2

	bool CheckMaxW(Process* p) {} //will be added in phase 2 used for migration


	virtual void addToReadyQueue(Process *process) //inserting a process to the RDY 
	{
		RDY.insertNode(process);
	}


	char getPtype()
	{
		return Ptype;
	}


	/*void CheckAndKill(int time) //traverse LinkedList to check if any process received kill Signal
	{
		Node <Process*>* traverse = RDY.head;  //ptr used to traverse RDY
		while (traverse->getNext())
		{
			if (time = process.killSig)
			{
				traverse->getItem()->setisFinished(true);
				traverse->getItem()->setTerminationTime(time);
				//kill children same way
				RDY.deleteNodes(traverse->getItem());
			}
			traverse = traverse->getNext();
		}
	}*/
	//virtual void ScheduleAlgo(int time) //phase 2
	//{
	//	//Check_Kill(time); //kill at this time step if signal received at rdy
	//	if (!RDY.isEmpty() && !getCurrRun()) //run empty and ready contains processes
	//	{
	//		Process* temp = RDY.getHead()->getItem(); //First Process In and the turn is on this Process to RUN
	//		RDY.deleteNode(); //deleting first Process as it is removed to RUN 
	//		setCurrRun(temp); //setting the RUN state to this process
	//	}
	//	else if (getCurrRun())  //run not empty
	//	{
	//		if (time <= 15)
	//		{
	//			Process* P = getCurrRun();
	//			getCurrRun()->setisBlocked(true);
	//			addtoBLK(P);
	//			setCurrRun(nullptr);
	//		}
	//		else if (time >= 20 && time <= 30)
	//		{
	//			addToReadyQueue(getCurrRun());
	//			setCurrRun(nullptr);
	//		}
	//		else if (time >= 50 && time <= 60)
	//		{
	//			Process* P = getCurrRun();
	//			addtoterminate(P);
	//			setCurrRun(nullptr);
	//		}
			//getCurrRun()->execute(time);
			//srand(0);
			//double forkP = rand() % 100; //generate a random forking probability
			//if (forkP == fork)         //compare the randomly generate dto the one from the input file
			//{
			//	int id = rand() % 99;
			//	/*while (id exists before) (binary search tree)
			//	{
			//		int id = rand() % 99;
			//	}*/
			//	int cput = getCurrRun()->gettimeRemaining();
			//	Process* P = new Process(id, time, cput, 0, []);  //7d yb2a ysalla7ha
			//}
			//kill if signal received at run
			///*if (time = kill_time && getCurrRun()->getPID() == kill_Id)
			//{
			//	getCurrRun()->setisFinished(true);
			//	getCurrRun()->setTerminationTime(time);
			//	RDY.deleteNodes(getCurrRun());
			//}*/
			/*if (getCurrRun()->getisBlocked())
				getCurrRun()->setisBlocked(true);*/
	/*	}
	}*/
	void print_rdy()
	{
		RDY.Print();
	}


	static void Loadp(ifstream& inputFile); //loading MaxW
	static void Loadpf(ifstream& inputFile); //loading Forking prob
	static void Loadkill(ifstream& inputFile); // loading kill signals

	~FCFS()
	{}
};



