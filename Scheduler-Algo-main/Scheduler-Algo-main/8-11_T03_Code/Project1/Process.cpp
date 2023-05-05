#include "Process.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
using namespace std; 

Process::Process(){}
Process::Process(int AT, int ID, int CT, int Num, SQueue<int> N, int d) :ArrivalTime(AT), PID(ID), CPUtime(CT), No_of_IO(Num), IO_queue(N), deadline(d) //changed by taha
{
    //AT = 0;
    //ID = 0;
    //CT = 0;
    timeRemaining = CT;
    orphanflag = false;
    isBlocked = false;
    isFinished = false;
    ResponseTime = -1;
    TerminationTime = -1;
    waitingTime = 0;
    TurnaroundDuration = 0;
    //No_of_IO = IO.size(); // no of times ha request input/output
    LCH = NULL;
    RCH = NULL;
}
void Process:: setorphanflag(bool f)
{
    orphanflag = f;
}
bool Process::getorphanflag()
{
    return orphanflag;
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
int Process::getDeadLine() // added by taha
{
    return deadline;
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

Process* Process::load(ifstream& inputFile) // changed by taha
{
    int AT, ID, CT, N, d;
    char c;
    SQueue<int> IO_queue;
    Process* Pp = NULL;
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
    inputFile >> d;
    Process* p = new Process(AT, ID, CT, N, IO_queue, d);
    return p;
}
SQueue<int> Process::getIOqueue() {
    return IO_queue;
}

///////////// Added by Amira /////////////
Process::Process(int AT, int ID, int CT) :ArrivalTime(AT), PID(ID), CPUtime(CT) //overloaded constructr for forking
{
    orphanflag = true;
    timeRemaining = CT;
    isBlocked = false;
    isFinished = false;
    ResponseTime = -1;
    TerminationTime = -1;
    waitingTime = 0;
    TurnaroundDuration = 0;
    //No_of_IO = IO.size(); // no of times ha request input/output
    LCH = nullptr;
    RCH = nullptr;
}
void Process::setRCH(Process* p)
{
    RCH = p;
}
void Process::setLCH(Process* p)
{
    LCH = p;
}
Process* Process::getRCH()
{
    return RCH;
}
Process* Process::getLCH()
{
    return LCH;
}
int Process::getCpuTime()
{
    return CPUtime;
}

ostream& operator<<(ostream& output, Process* p1)
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
