#include "Process.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
using namespace std; 

Process::Process(){}
Process::Process(int AT, int ID, int CT, int Num, SQueue<int> N) :ArrivalTime(AT), PID(ID), CPUtime(CT), No_of_IO(Num), IO_queue(N)
{
    AT = 0;
    ID = 0;
    CT = 0;
    timeRemaining = CPUtime;
    isBlocked = false;
    isFinished = false;
    ResponseTime = -1;
    TerminationTime = -1;
    waitingTime = 0;
    TurnaroundDuration = 0;
    //No_of_IO = IO.size(); // no of times ha request input/output
    LCH = NULL;
    RCH = NULL;
    //lch_ID = -1;  //1st child ID set to -1 if no children added by Amira
    //rch_ID = -1;   //1st child ID set to -1 if no children added by Amira
    //ch_count = 0; //number of children; set to 0 by default added by Amira
}

int Process::getPID() 
{
    return PID;
}

int Process::getArrivalTime() 
{
    return ArrivalTime;
}

int Process::getResponseTime() 
{
    return ResponseTime;
}


void Process::settimeRemaining(int time)
{
    timeRemaining = time;
}

int Process::gettimeRemaining()
{
    return timeRemaining;
}


void Process::setResponseTime(int time) 
{
    ResponseTime = time;
}

int Process::getTerminationTime() 
{
    return TerminationTime;
}

void Process::setTerminationTime(int time) 
{
    TerminationTime = time;
}

int Process::getTurnaroundDuration() 
{
    return TerminationTime - ArrivalTime;
}

int Process::getWaitingTime() 
{
    return waitingTime;
}

bool Process::getisBlocked() 
{
    return isBlocked;
}
void Process::setisBlocked(bool it)
{
    isBlocked =it;
}
void Process::setisFinished(bool it)
{
    isFinished = it;
}
bool Process::getisFinished()
{
    return isFinished;
}

void Process::execute(int currentTimeStep)  // decrement cpu at each time step 
{
    if (timeRemaining > 0) {
        timeRemaining--;
        if (timeRemaining == 0) 
        {
            isFinished = true;
            TerminationTime = currentTimeStep;
        }
    }
}

Process *Process:: load(ifstream& inputFile)
{
    int AT, ID, CT, N;
    char c;
    SQueue<int> IO_queue;
    Process * Pp = NULL ;
        inputFile >> AT;
        inputFile >> ID;
        inputFile >> CT;
        inputFile >> N;
        for (int i = 0; i < N; i++)
        {
            int IO_R = 0, IO_D = 0;
            inputFile >> IO_R;
            inputFile >> c;
            inputFile >> IO_D;
            IOpairs = new SNode<int>(IO_R, IO_D);
            IO_queue.enqueue({ IOpairs });
        }
        Process *p= new Process(AT, ID, CT, N, IO_queue);
     return p;
}


   
/*void Process::requestIO(int currentTimeStep, int inputRequestTime) // phase 2 can be deleted
{
    isBlocked = true;
    this->inputRequestTime = inputRequestTime;
}

void Process::completeIO(int currentTimeStep) // phase 2 can be deleted
{
    isBlocked = false;
    int timeSpentWaiting = currentTimeStep - inputRequestTime;
    waitingTime += timeSpentWaiting;
    timeRemaining -= InputDuration;
}
*/

ostream& operator<<(ostream& output, Process* p1) // overloading for ID in phase 1 / more to overload in phase 2
{
    output << p1->PID << " ";
    /*output << "Arrival time: " << p1->ArrivalTime << "\n";
    output << "CPU time: " << p1->CPUtime << "\n";
    output << "Response time: " << p1->ResponseTime << "\n";
    output << "Termination time: " << p1->TerminationTime << "\n";
    output << "Turnaround duration: " << p1->getTurnaroundDuration() << "\n";
    output << "Waiting time: " << p1->waitingTime << "\n";*/
   return output;
}
Process::~Process(){}
