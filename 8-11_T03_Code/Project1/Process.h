#pragma once
#include "StructQueue.h"
#include <iostream>
#include <fstream>
using namespace std;

class Process
{
private:
    int PID;
    bool iskilled; //to adjust waiting time for killed processes
    int totalIO_D; // added for output file
    int totalIO_R; // added for checking the done IO requests to check for others in further executions 
    int firsttime; //to check response time  
    int ArrivalTime;
    int ResponseTime;
    int CPUtime;
    int timeblk; //used in handling blk
    int numIO; //number of times a process asks for IO
    int TerminationTime;
    int TurnaroundDuration;
    int waitingTime;
    int deadline;
    SNode <int>* IOpairs;
    SQueue<int> IO_queue;
    int timeRemaining; //CT - number of timesteps a process was executed
    int No_of_IO; //no of times user request input or output
    bool isBlocked; //if process state is blocked
    bool isFinished; //process finished execution
    bool orphanflag; //flag for forked processes
    Process* Parent;
    Process* LCH;
    Process* RCH;

public:
    Process();
    Process(int AT, int ID, int CT, int Num, SQueue<int> N, int d);
    Process(int AT, int ID, int CT, Process* parent); //overloaded constructor for forking
    Process* load(ifstream& inputFile);
    void execute(int currentTimeStep);
    friend ostream& operator<<(ostream& output, Process* p1);
    ~Process();

    /////////////////////////////////////////
    //				Getters				   //
    /////////////////////////////////////////
    Process* getParent(); //getter for forked child to parent
    Process* getRCH();
    Process* getLCH();
    SQueue<int>* getIOqueue();
    int getCpuTime();
    int getnumIO();
    int getblktime();
    bool getiskilled();
    int getfirsttime();
    int getTotalIO_D();
    int getPID();
    bool getorphanflag();
    int getTotalIO_R();
    int getArrivalTime();
    int getResponseTime();
    int getTurnaroundDuration();
    int getWaitingTime();
    int getDeadLine();
    bool getisBlocked();
    int getTerminationTime();
    int gettimeRemaining();
    bool getisFinished();

    /////////////////////////////////////////
    //				setters				   //
    /////////////////////////////////////////
    void setiskilled(bool t);
    void setnumIO(int t);
    void setblktime(int t);
    void setwaitingtime(int t);
    void setTurnaroundDuration(int t);
    void setfirsttime(int t);
    void setTotalIO_D(int t);
    void setTotalIO_R(int t);
    void setorphanflag(bool f);
    void settimeRemaining(int time);
    void setResponseTime(int time);
    void setTerminationTime(int time);
    void setisBlocked(bool it);
    void setisFinished(bool it);
    void setRCH(Process* p);
    void setLCH(Process* p);

};