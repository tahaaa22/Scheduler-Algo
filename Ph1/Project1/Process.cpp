#include "Process.h"

#include "Process.h"

Process::Process(int id, int arrivalTime, int cpuTime, int inputDuration , int No)
{
    PID = id;
    ArrivalTime = arrivalTime;
    CPUtime = cpuTime;
    InputDuration = inputDuration;
    timeRemaining = cpuTime;
    isBlocked = false;
    isFinished = false;
    ResponseTime = -1;
    TerminationTime = -1;
    waitingTime = 0;
    inputRequestTime = -1;
    no = No; // no of times ha request input/output

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
void Process::setno_ofinput(int n)
{
    no = n;
}

int Process::getNo_ofinput()
{
    return no;
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

void Process::requestIO(int currentTimeStep, int inputRequestTime) 
{
    isBlocked = true;
    this->inputRequestTime = inputRequestTime;
}

void Process::completeIO(int currentTimeStep) 
{
    isBlocked = false;
    int timeSpentWaiting = currentTimeStep - inputRequestTime;
    waitingTime += timeSpentWaiting;
    timeRemaining -= InputDuration;
}

/*ostream& operator<<(ostream& output, Process* p1)
{
    output << "PID: " << p1->PID << "\n";
    output << "Arrival time: " << p1->ArrivalTime << "\n";
    output << "CPU time: " << p1->CPUtime << "\n";
    output << "Input/Output duration: " << p1->InputDuration << "\n";
    output << "Response time: " << p1->ResponseTime << "\n";
    output << "Termination time: " << p1->TerminationTime << "\n";
    output << "Turnaround duration: " << p1->getTurnaroundDuration() << "\n";
    output << "Waiting time: " << p1->waitingTime << "\n";
    output << "Blocked: " << (p1->getisBlocked() ? "Yes" : "No") << "\n";
    output << "Finished: " << (p1->getisFinished() ? "Yes" : "No") << "\n";
    return output;
}*/

Process::~Process(){}