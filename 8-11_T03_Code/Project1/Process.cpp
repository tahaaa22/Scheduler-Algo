#include "Process.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
using namespace std;

Process::Process() {} //defult constructor

Process::Process(int AT, int ID, int CT, int Num, SQueue<int> N, int d) :ArrivalTime(AT), PID(ID), CPUtime(CT), numIO(Num), IO_queue(N), deadline(d) //changed by taha
{
    Parent = nullptr; // no forking intially
    timeblk = 0;  // time in which the process will go to the block
    firsttime = 0;  // intially it hasn't runned yet
    timeRemaining = CT; // intially time rem aining =cpu
    orphanflag = false; //not orphan intially
    isBlocked = false; //not blocked intially
    isFinished = false; //not finished intially
    iskilled = false; //not killed intially
    ResponseTime = 0; //no response time intially
    TerminationTime = 0; //not finished intially
    waitingTime = -1; //will be calculated later on
    TurnaroundDuration = 0; //no trt time intially
    LCH = NULL; //no Left child intially
    RCH = NULL; //no right child intially
}

///Constructor for forked process

Process::Process(int AT, int ID, int CT, Process* parent) :ArrivalTime(AT), PID(ID), CPUtime(CT), Parent(parent) //overloaded constructr for forking
{
    firsttime = 0;
    orphanflag = true; //forked child so set this flag true to avoid migration and stealing
    timeRemaining = CT;
    isBlocked = false; //not blocked intially
    isFinished = false; //not finished intially
    ResponseTime = 0; //no response time intially
    TerminationTime = 0; //not finished intially
    waitingTime = -1; //will be calculated later on
    TurnaroundDuration = 0; //no trt time intially
    LCH = NULL; //no Left child intially
    RCH = NULL; //no right child intially
}

//load fn
Process* Process::load(ifstream& inputFile)
{
    int AT, ID, CT, N, d;
    char c;
    SQueue<int> IO_queue;
    Process* Pp = NULL;
    inputFile >> AT; //Load Arrival time
    inputFile >> ID; // load id 
    inputFile >> CT; // load cpu
    inputFile >> N; // load no of time it will request blk
    numIO = N; // this number used to know how many IO resources needed in the blk function
    for (int i = 0; i < N; i++) //load the block queue
    {
        if (ID == 7)
            int t = 0;
        int IO_R = 0, IO_D = 0;
        inputFile >> IO_R;  //load request time
        inputFile >> c;     //load the comma
        inputFile >> IO_D;  //load the duration 
        IOpairs = new SNode<int>(IO_R, IO_D);
        IO_queue.enqueue(IOpairs);
    }
    if (N == 0) //no pairs
    {
        IOpairs = new SNode<int>(0, 0);
        IO_queue.enqueue(IOpairs); //enqueue 0 and 0 as it won't request
    }
    inputFile >> d; //load the deadline
    Process* p = new Process(AT, ID, CT, N, IO_queue, d); //create the process
    return p;
}

void Process::execute(int currentTimeStep)  // decrement cpu at each time step 
{
    if (timeRemaining > 0)
    {
        timeRemaining--;
        if (timeRemaining == 0)
        {
            isFinished = true; //if finished set the bool true & set termanation time
            TerminationTime = currentTimeStep;
        }
    }
}

//operator overloading of the peocess
ostream& operator<<(ostream& output, Process* p1)
{
    output << p1->PID << " "; //print the id of the process
    return output;
}

Process::~Process()
{
    //deallocating 
    delete Parent;
    delete LCH;
    delete RCH;
}


//////////////////////////////////////// GETTTERS//////////////////////////////////////
Process* Process::getParent() //get parent
{
    return Parent;
}
Process* Process::getRCH() //Get right child
{
    return RCH;
}
Process* Process::getLCH() //get left child
{
    return LCH;
}
SQueue<int>* Process::getIOqueue()
{
    return &IO_queue; //get the io queue to check for the block
}
int Process::getnumIO()
{
    return numIO; //Get no of ios
}
int Process::getblktime()
{
    return timeblk;
}
bool Process::getiskilled()
{
    return iskilled;
}
int Process::getfirsttime()
{
    return firsttime;
}
int Process::getTotalIO_D()
{
    return totalIO_D;
}
int Process::getTotalIO_R()
{
    return totalIO_R;
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
int Process::getDeadLine()
{
    return deadline;
}
int Process::gettimeRemaining()
{
    return timeRemaining;
}
int Process::getTerminationTime()
{
    return TerminationTime;
}
int Process::getTurnaroundDuration()
{
    return TurnaroundDuration;
}
int Process::getWaitingTime()
{
    return waitingTime;
}
bool Process::getisBlocked()
{
    return isBlocked;
}
bool Process::getisFinished()
{
    return isFinished;
}
int Process::getCpuTime()
{
    return CPUtime;
}


//////////////////////////////////////////// Setters///////////////////////////////////////

void Process::setiskilled(bool t)
{
    iskilled = t;
}
void Process::setnumIO(int t)
{
    numIO = t;
}
void Process::setblktime(int t)
{
    timeblk = t;
}
void Process::setwaitingtime(int t)
{
    waitingTime = t;
}
void Process::setTurnaroundDuration(int t)
{
    TurnaroundDuration = t;
}
void Process::setfirsttime(int t)
{
    firsttime = t;
}
void Process::setTotalIO_D(int t)
{
    totalIO_D = t;
}
void Process::setTotalIO_R(int t)
{
    totalIO_R = t;
}
void Process::setorphanflag(bool f)
{
    orphanflag = f;
}
void Process::settimeRemaining(int time)
{
    timeRemaining = time;
}
void Process::setResponseTime(int time)
{
    ResponseTime = time;
}
void Process::setTerminationTime(int time)
{
    TerminationTime = time;
}
void Process::setisBlocked(bool it)
{
    isBlocked = it;
}
void Process::setisFinished(bool it)
{
    isFinished = it;
}
void Process::setRCH(Process* p)
{
    RCH = p;
}
void Process::setLCH(Process* p)
{
    LCH = p;
}