#pragma once
#include <fstream>
#include "Processor.h"
#include "LinkedList.h"
#include "StructQueue.h"

class FCFS : public Processor
{
	//The FCFS is a queue but because of the kill signal that will require accessing 
	// a process in the middle of the queue the structure is now a LinkedList
private:
	LinkedList<Process*> RDY;
	double fork; //fork probability
	SQueue<int> killSig; //Kill Signal Time 
	int MaxW; //to use for check in migration
public:
	void  Check_Kill(int timestep) {} //will be added in phase 2
	bool CheckMaxW(Process* p) {} //will be added in phase 2 used for migration
	virtual void addToReadyQueue(Process* process) //inserting a process to the RDY 
	{
		RDY.insertNode(process);
	}
	virtual Process* getNextProcess()   //getting the process that should be moved to RUN 
	{
		if (!RDY.isEmpty())
		{
			return RDY.GetTail();
		}
		else
		{
			return nullptr;
		}
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

	void ScheduleAlgo(int time)
	{
		//Check_Kill(time); //kill at this time step if signal received at rdy
		if (!RDY.isEmpty() && !getCurrRun()) //run empty and ready contains processes
		{
			Process* temp = RDY.GetTail(); //First Process In and the turn is on this Process to RUN
			RDY.deleteNode(); //deleting first Process as it is removed to RUN 
			setCurrRun(temp); //setting the RUN state to this process
		}
		else if (getCurrRun())  //run not empty
		{
			getCurrRun()->execute(time);
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
		}
	}
	virtual void print_rdy()
	{
		RDY.Print();
	}
	void loadp()
	{
		// open the file
		ifstream inputFile("filename.txt");
		int int_number = 6; // MAXW MAKANO 6
		int NO_MAXW;
		int current_int_number = 0;

		if (inputFile.fail())
		{
			// Check if file is not opened
			exit(1); // terminates program
		}
		else
		{
			while (inputFile >> NO_MAXW)
			{
				current_int_number++;
				if (current_int_number == int_number)
				{
					MaxW = NO_MAXW;
					break;

				}
			}
		}
		// close the file
		inputFile.close();
	}
};
